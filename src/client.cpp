#pragma once
#include <ixwebsocket/IXWebSocket.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <nlohmann/json.hpp>

class NetworkClient {
    ix::WebSocket ws;
    std::mutex qMutex;
    std::queue<std::string> moveQueue;
    bool connected = false;

public:
    NetworkClient(const std::string& url) {
        ws.setUrl(url);

        ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Message) {
                auto jsonMsg = nlohmann::json::parse(msg->str);
                if (jsonMsg["type"] == "move") {
                    std::lock_guard<std::mutex> lock(qMutex);
                    moveQueue.push(jsonMsg["move"]);
                }
            }
        });
    }

    void connect() {
        ws.start();
        connected = true;
    }

    void disconnect() {
        if (connected) ws.stop();
        connected = false;
    }

    std::string requestRandomRoom() {
        nlohmann::json req = { {"type", "randomRoom"} };
        ws.send(req.dump());

        while (true) {
            ix::WebSocketMessagePtr msg = ws.getNextMessage();
            if (!msg) continue;
            if (msg->type == ix::WebSocketMessageType::Message) {
                auto jsonMsg = nlohmann::json::parse(msg->str);
                if (jsonMsg["type"] == "randomRoom") return jsonMsg["roomId"];
            }
        }
    }

    void joinRoom(const std::string& roomId) {
        nlohmann::json req = { {"type", "join"}, {"roomId", roomId} };
        ws.send(req.dump());
    }

    void leaveRoom() {
        nlohmann::json req = { {"type", "leave"} };
        ws.send(req.dump());
    }

    void sendMove(const std::string& move) {
        nlohmann::json req = { {"type", "move"}, {"move", move} };
        ws.send(req.dump());
    }

    bool hasOpponentMove() {
        std::lock_guard<std::mutex> lock(qMutex);
        return !moveQueue.empty();
    }

    std::string getOpponentMove() {
        std::lock_guard<std::mutex> lock(qMutex);
        if (moveQueue.empty()) return "";
        std::string mv = moveQueue.front();
        moveQueue.pop();
        return mv;
    }
};
