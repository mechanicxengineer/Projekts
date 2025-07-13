
#pragma once

#include <string>
#include <vector>

struct Task {
    std::string description;
    bool done;
};

std::string GetDocumentsPath();
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
std::vector<Task> LoadTasksFromFile(const std::string& filename);