
#include "task.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

std::string GetDocumentsPath() {
    const char* userProfile = std::getenv("USERPROFILE");
    return std::string(userProfile) + "\\Documents\\tasks.txt";
}

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream ostream(filename);
    ostream << tasks.size();

    for (const Task& task : tasks) {
        std::string description = task.description;
        std::replace(description.begin(), description.end(), ' ', '_');

        ostream << "\n" << description << ' ' << task.done;
    }
}

std::vector<Task> LoadTasksFromFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        return std::vector<Task>();
    }

    std::vector<Task> tasks;
    std::ifstream istream(filename);

    int size;
    istream >> size;

    for (int i = 0; i < size; ++i) {
        std::string description;
        bool done;

        istream >> description >> done;
        std::replace(description.begin(), description.end(), '_', ' ');
        tasks.push_back(Task{description, done});
    }

    return tasks;
}