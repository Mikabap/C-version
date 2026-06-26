#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <coroutine>
#include <memory>
#include <cstdint>
#include <iomanip>

struct WallNode {
    int16_t x, y;
    uint8_t width, height;
};

enum class TileState : uint8_t { Empty = 0, Wall = 1, Pocket = 2 };

struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

struct Delay {
    int milliseconds;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        std::thread([h, this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            h.resume();
        }).detach();
    }
    void await_resume() {}
};

class MazeEngine {
private:
    int width, height;
    std::vector<TileState> map_grid;
    std::mt19937 generator;

public:
    MazeEngine(int w, int h, uint32_t seed) : width(w), height(h), map_grid(w * h, TileState::Empty), generator(seed) {}

    Task generate(int seed_count) {
        for (int i = 0; i < seed_count; ++i) {
            int start_x = std::uniform_int_distribution<int>(1, width - 2)(generator);
            int start_y = std::uniform_int_distribution<int>(1, height - 2)(generator);

            if (map_grid[start_y * width + start_x] == TileState::Empty) {
                grow_path(start_x, start_y);
            }

            if (i % 5 == 0) co_await Delay{8}; 
        }
    }

private:
    void grow_path(int x, int y) {
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int dir = std::uniform_int_distribution<int>(0, 3)(generator);

        while (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
            map_grid[y * width + x] = TileState::Wall;

            float roll = std::uniform_real_distribution<float>(0.0f, 1.0f)(generator);
            if (roll < 0.75f) {
                x += directions[dir][0];
                y += directions[dir][1];
            } else if (roll < 0.95f) {
                dir = (dir + 1) % 4;
                x += directions[dir][0];
                y += directions[dir][1];
            } else {
                break;
            }
        }
    }
};

int main() {
    MazeEngine engine(40, 40, 1337);
    
    engine.generate(100);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}
