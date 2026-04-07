#pragma once
#include "gameobject.h"
#include "box.h"
#include "player.h"
#include <sgg/graphics.h>

class Collectible : public Box, public GameObject {
private:
    graphics::Brush m_brush;
    bool m_collected = false;

public:
    Collectible(float x, float y, float size, const std::string& texture)
        : Box(x, y, size, size), GameObject("Collectible") {
        m_brush.texture = texture;
        m_brush.outline_opacity = 0.0f;
    }

    void update(float dt) override {
        // No specific logic for collectibles right now.
    }

    void draw() override {
        if (!m_collected) {
            graphics::drawRect(m_pos_x1, m_pos_y1, m_width, m_height, m_brush);
        }
    }

    bool checkCollision(Player* player) {
        if (!m_collected && checkCollision1(this, *player)) {
            m_collected = true;
            return true;
        }
        return false;
    }

    bool isCollected() const { return m_collected; }
};
