#pragma once
#include <math.h>
#include <algorithm>

struct Box
{

    float m_pos_x1 = 0.0f;
    float m_pos_y1 = 0.0f;
    float m_pos_x2 = 0.0f;
    float m_pos_y2 = 0.0f;
    float m_width = 1.0f;
    float m_height = 1.0f;

    //check collision for player1 
    bool checkCollision1(const Box* a, const Box& b) {
        if (a->m_pos_x1 + a->m_width < b.m_pos_x1 || b.m_pos_x1 + b.m_width < a->m_pos_x1 ||
            a->m_pos_y1 + a->m_height < b.m_pos_y1 || b.m_pos_y1 + b.m_height < a->m_pos_y1) {
            return false;
        }

        return true;
    }
    //check collision for player2 
    bool checkCollision2(const Box* a, const Box& b) {
        if (a->m_pos_x2 + a->m_width < b.m_pos_x2 || b.m_pos_x2 + b.m_width < a->m_pos_x2 ||
            a->m_pos_y2 + a->m_height < b.m_pos_y2 || b.m_pos_y2 + b.m_height < a->m_pos_y2) {
            return false;
        }

        return true; 
    }

    float intersectDown1(Box& other)
    {
        if (fabs(m_pos_x1 - other.m_pos_x1) * 2.0f >= (m_width + other.m_width) || m_pos_y1 > other.m_pos_y1)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y1 - (other.m_height / 2.0f) - m_pos_y1 - (m_height / 2.0f));
    }

    float intersectUp1(Box& other)
    {
        if (fabs(m_pos_x1 - other.m_pos_x1) * 2.0f >= (m_width + other.m_width) || m_pos_y1 < other.m_pos_y1)
            return 0.0f;

        return std::min<float>(0.0f, m_pos_y1 + (m_height / 2.0f) - (other.m_pos_y1 - (other.m_height / 2.0f)));

    }

    float intersectSideways1(Box& other)
    {
        if (fabs(m_pos_y1 - other.m_pos_y1) * 2.0f >= (m_width + other.m_width))
            return 0.0f;
        if (m_pos_x1 > other.m_pos_x1)
            return std::max<float>(0.0f, other.m_pos_x1 + (other.m_width / 2.0f) - m_pos_x1 + (m_width / 2.0f));
        else
            return std::min<float>(0.0f, other.m_pos_x1 - (other.m_width / 2.0f) - m_pos_x1 - (m_width / 2.0f));
    }

    float intersectDown2(Box& other)
    {
        if (fabs(m_pos_x2 - other.m_pos_x2) * 2.0f >= (m_width + other.m_width) || m_pos_y2 > other.m_pos_y2)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y2 - (other.m_height / 2.0f) - m_pos_y2 - (m_height / 2.0f));
    }

    float intersectUp2(Box& other)
    {
        if (fabs(m_pos_x2 - other.m_pos_x2) * 2.0f >= (m_width + other.m_width) || m_pos_y2 < other.m_pos_y2)
            return 0.0f;

        return std::min<float>(0.0f, m_pos_y2 + (m_height / 2.0f) - (other.m_pos_y2 - (other.m_height / 2.0f)));

    }

    float intersectSideways2(Box& other)
    {
        if (fabs(m_pos_y2 - other.m_pos_y2) * 2.0f >= (m_width + other.m_width))
            return 0.0f;
        if (m_pos_x2 > other.m_pos_x2)
            return std::max<float>(0.0f, other.m_pos_x2 + (other.m_width / 2.0f) - m_pos_x2 + (m_width / 2.0f));
        else
            return std::min<float>(0.0f, other.m_pos_x2 - (other.m_width / 2.0f) - m_pos_x2 - (m_width / 2.0f));
    }
    Box() {}

    Box(float x, float y, float w, float h)
        : m_pos_x1(x), m_pos_x2(x), m_pos_y1(y), m_pos_y2(y), m_width(w), m_height(h) {}
};

