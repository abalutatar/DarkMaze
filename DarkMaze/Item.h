#pragma once
#include <glm.hpp>

enum class ItemType {
    KEY,
    BATTERY
};

class Item {
public:
    glm::vec3 position;   // pozycja w œwiecie
    ItemType type;        // typ obiektu
    bool collected;       // czy zosta³ zebrany

    Item(glm::vec3 pos, ItemType t)
        : position(pos), type(t), collected(false) {
    }
};
