#pragma once
#include <cmath>

namespace crowdsim {
constexpr float eps = 1e-12f;

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_) : x(x_), y(y_) {}

    constexpr Vec2 operator+(Vec2 o) const { return {x + o.x, y + o.y}; }
    constexpr Vec2 operator-(Vec2 o) const { return {x - o.x, y - o.y}; }
    constexpr Vec2 operator*(float s) const { return {x * s, y * s}; }
    constexpr Vec2 operator/(float s) const { return {x / s, y / s}; }
    constexpr Vec2 operator-() const { return {-x, -y}; }

    constexpr Vec2& operator+=(Vec2 o) { x += o.x; y += o.y; return *this; }
    constexpr Vec2& operator-=(Vec2 o) { x -= o.x; y -= o.y; return *this; }
    constexpr Vec2& operator*=(float s) { x *= s; y *= s; return *this; }

    constexpr float dot(Vec2 o) const { return x * o.x + y * o.y; }

    constexpr float length_sq() const { return x * x + y * y; }

    float length() const { return std::sqrt(length_sq()); }

    Vec2 truncated(float max_len) const {
        if (max_len <= 0) return {};
        float l_sq = length_sq();
        if (l_sq > max_len * max_len) {
            float n = max_len / std::sqrt(l_sq);
            return *this * n;
        }
        return *this;
    }

    Vec2 normalized() const {
        float l_sq = length_sq()
        if (l_sq < 1e-12f) {
            return {};
        }
        return *this / std::sqrt(l_sq);
    }
};

constexpr Vec2 operator*(float s, Vec2 v) { return v * s; }

inline float distance(Vec2 a, Vec2 b) { return (a - b).length(); }
constexpr float distance_sq(Vec2 a, Vec2 b) { return (a - b).length_sq(); }


struct Pedestrian {
    Vec2  pos{};
    Vec2  vel{};
    Vec2  goal{};
    float radius = 0.3f;
    float v_pref = 1.3f;
};


struct Robot {
    Vec2  pos{};
    float theta = 0.0f;
    float v = 0.0f;
    float omega = 0.0f;
    Vec2  goal{};
    float radius = 0.3f;

    Vec2 velocity() const {
        return Vec2(std::cos(theta), std::sin(theta)) * v;
    }
};


struct Wall {
    Vec2 a{};
    Vec2 b{};
};

float distance_to_segment(Vec2 p, Vec2 a, Vec2 b);

// ---------------------------------------------------------------------------
// Конфигурация
// ---------------------------------------------------------------------------

// Параметры Social Force Model (Helbing & Molnar, 1995).
struct SfmParams {
    float tau    = 0.5f;   // время релаксации к желаемой скорости, с
    float A_ped  = 2.0f;   // амплитуда отталкивания между пешеходами
    float B_ped  = 0.3f;   // характерная дальность отталкивания, м
    float A_wall = 10.0f;  // от стен отталкивает сильнее: в стену не вдавишься
    float B_wall = 0.2f;
    // Дальше cutoff сила меньше exp(-5) ~ 0.7% от A. Задаёт размер клетки grid.
    float cutoff = 1.5f;
};

// Ограничения привода робота. Одинаковы для всех миров, поэтому в конфиге,
// а не в самой структуре Robot.
struct RobotLimits {
    float v_max     = 1.0f;   // м/с, вперёд
    float v_min     = 0.0f;   // м/с, назад (0 = задний ход запрещён)
    float omega_max = 1.0f;   // рад/с
};

// Все поля со значениями по умолчанию: из Python можно будет менять один
// параметр, не перечисляя остальные.
struct WorldConfig {
    float dt = 0.1f;

    int   num_pedestrians = 20;
    float arena_half_w    = 10.0f;   // мир: [-half_w, half_w] x [-half_h, half_h]
    float arena_half_h    = 10.0f;

    float ped_radius   = 0.3f;
    float ped_v_pref   = 1.3f;
    float robot_radius = 0.3f;

    int   max_steps      = 500;   // лимит шагов эпизода
    float goal_tolerance = 0.3f;  // ближе этого к цели - считается достигнутой

    SfmParams   sfm{};
    RobotLimits robot{};
};

}  // namespace crowdsim
