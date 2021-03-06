#include <catch2/catch.hpp>
#include <PacManAI.hpp>
#include <limits>

TEST_CASE("Find pellet closest to PacMan", "[AI]") {
    using namespace pacman;
    PacManAI AI;
    using TestData = std::tuple<GridPosition, std::vector<GridPosition>, GridPosition>;
    auto data = GENERATE(
                TestData{{5, 5}, {{5, 6}}, {5, 6}},
                TestData{{5, 5}, {{5, 5}}, {5, 5}},
                TestData{{5, 5}, {{0, 0}, {5, 6}}, {5, 6}},
                TestData{{5, 5}, {{6, 5}, {5, 6}}, {6, 5}},
                TestData{{5, 5}, {{6, 6}, {5, 6}}, {5, 6}}
                );

    CHECK(AI.pelletClosestToPacman(std::get<0>(data), std::get<1>(data)) == std::get<2>(data));
}

TEST_CASE("Is valid move", "[AI]") {
    using namespace pacman;
    using TestData = std::tuple<PacManAI::Move, bool>;
    auto data = GENERATE(
                TestData{{Direction::RIGHT, {13, 23}}, true},
                TestData{{Direction::LEFT, {13, 23}}, false}, // opposite direction
                TestData{{Direction::RIGHT, {13, 22}}, false} // wall
                );

    PacManAI AI;
    CHECK(AI.isValidMove(std::get<0>(data)) == std::get<1>(data));
}

TEST_CASE("Is optimal direction", "[AI]") {
    using namespace pacman;
    using TestData = std::tuple<std::array<PacManAI::Move, 4>, Direction>;
    auto makeMove = [](double distance, Direction d) {
        return PacManAI::Move{d, {0, 0}, distance};
    };
    const auto infinity = std::numeric_limits<double>::infinity();

    auto data = GENERATE_REF(
        TestData{{makeMove(1, Direction::LEFT),
                  makeMove(1, Direction::RIGHT),
                  makeMove(1, Direction::UP),
                  makeMove(1, Direction::DOWN)},
                  Direction::LEFT},
        TestData{{makeMove(3, Direction::LEFT),
                  makeMove(2, Direction::RIGHT),
                  makeMove(2, Direction::UP),
                  makeMove(3, Direction::DOWN)},
                  Direction::RIGHT},
        TestData{{makeMove(3, Direction::LEFT),
                  makeMove(2, Direction::RIGHT),
                  makeMove(1, Direction::UP),
                  makeMove(3, Direction::DOWN)},
                  Direction::UP},
        TestData{{makeMove(infinity, Direction::LEFT),
                  makeMove(infinity, Direction::RIGHT),
                  makeMove(infinity, Direction::UP),
                  makeMove(infinity, Direction::DOWN)},
                  Direction::LEFT},
        TestData{{makeMove(infinity, Direction::LEFT),
                  makeMove(42, Direction::RIGHT),
                  makeMove(infinity, Direction::UP),
                  makeMove(infinity, Direction::DOWN)},
                  Direction::RIGHT});

        PacManAI AI;
        CHECK(AI.optimalDirection(std::get<0>(data)) == std::get<1>(data));
}
