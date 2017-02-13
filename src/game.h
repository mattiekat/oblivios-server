#pragma once
#include <cstdint>

struct Player;
struct Thread;

namespace nolhmann {class json;}
using Json = nlohmann::json;

/**
 * Abstract the game such that the server could handle more than one
 */
class Game {
    /// The memory warriors will run in
    uint8_t ram[0x10000];
    /// The pid of last modification
    uint8_t pid[0x10000];
    uint64_t cycle;

    const uint16_t cycles_per_turn;
    const int64_t max_cycles;
    const uint32_t score_for_killing_thread;
    const uint32_t score_for_killing_process;
    const float score_for_owning_ram;

    /// The number of players in a given game
    const uint8_t num_players;
    /// Array of players, index i is the player with pid i + 1
    Player* players;

    /**
     * Send the inital information about game state.
     * @param log The log stream
     */
    void sendInit(std::ostream& log);

    /**
     * Executes the next instruction.
     * @param thread The thread to run.
     * @param log The output stream for updates.
     * @return True if successful, false if not.
     */
    bool execIns(Thread& thread, std::ostream& log);

public:
    Game() = delete;
    Game(const Json& config);
    ~Game();

    void run(std::ostream& log);

    /// @warning This prints a lot of stuff
    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};