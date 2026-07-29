#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    // Your code here
    std::vector<std::unique_ptr<string>> out;
    out.reserve(items.size());
    for (auto& t : items) {
        out.push_back(std::make_unique<string>(*t.get()));
    }
    return out;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    // Your code here
    std::vector<std::shared_ptr<string>> out;
    std::map<std::string_view, std::shared_ptr<string>> mapa;
    out.reserve(items.size());
    for (auto& t : items) {
        auto& str = *t.get();
        if (mapa.count(str)) {
            out.push_back(mapa[str]);
        } else {
            out.push_back(std::make_shared<string>(str));
            mapa[str] = out.back();
        }
    }
    return out;
}
