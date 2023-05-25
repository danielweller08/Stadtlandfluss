#include <gtest/gtest.h>
#include <stadt_land_fluss/controller.hpp>

using namespace StadtLandFluss;


// Test the get_games function
TEST(ControllerTest, GetGamesTest) {
    Controller controller;
    int userToken1 = controller.get_userToken();
    int userToken2 = controller.get_userToken();
    // Create two games and check if they are returned by get_games
    Board board = controller.create_game("Alice", userToken1);
    Board board_ = controller.create_game("Bob", userToken2);
    vector<Board> games = controller.get_games();
    EXPECT_EQ(2, games.size());
}

// Test the get_userToken function
TEST(ControllerTest, GetUserTokenTest) {
    Controller controller;
    int token = controller.get_userToken();
    EXPECT_GE(token, 1000000);
    EXPECT_LE(token, 9999999);
}

// Test if player is assigned to a game correctly
TEST(ControllerTest, CreateUserAndJoinGame) {
    Controller controller;
    int userToken = controller.get_userToken();
    std::string userName = "Alice";

    // Create a game and join as the user
    Board board = controller.create_game(userName, userToken);
    EXPECT_EQ(1, board.get_players().size());
    
    const map<string, int> players = board.get_players();

    // Iterate through players to find the correct user
    bool foundUser = false;
    for (auto elem : players) {
        if (elem.first == userName) {
            foundUser = true;
        }
    }
    ASSERT_TRUE(foundUser);
}

// Test the create_game function when the user is already in a game
TEST(ControllerTest, CreateGameWithUserInGameTest) {
    Controller controller;
    // Create a game with a user
    int userToken = controller.get_userToken();
    controller.create_game("Alice", userToken);
    // Create another game with the same user
    EXPECT_THROW(controller.create_game("Alice", userToken), std::invalid_argument);
}

// Test create_game gameIDs
TEST(ControllerTest, CreateGameUniqueIDTest) {
    Controller controller;
    // Create multiple games
    int userToken1 = controller.get_userToken();
    Board board1 = controller.create_game("Alice", userToken1);

    int userToken2 = controller.get_userToken();
    Board board2 = controller.create_game("Bob", userToken2);

    int userToken3 = controller.get_userToken();
    Board board3 = controller.create_game("Charlie", userToken3);

    // Ensure the game IDs are unique and incremented correctly
    EXPECT_NE(board1.get_id(), board2.get_id());
    EXPECT_NE(board2.get_id(), board3.get_id());
    EXPECT_EQ(board1.get_id() + 1, board2.get_id());
    EXPECT_EQ(board2.get_id() + 1, board3.get_id());
}

// Test the join_game function
TEST(ControllerTest, JoinGameTest) {
    Controller controller;
    // Create a game
    int userToken1 = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken1);
    // Join the game with a different user
    int userToken2 = controller.get_userToken();
    board = controller.join_game(board.get_id(), "Bob", userToken2);
    EXPECT_EQ(2, board.get_players().size());
}

// Test join_game exceptions
TEST(ControllerTest, JoinGameWithUserAlreadyInGameTest) {
    Controller controller;
    // Create two games and join with a user in the first game
    int userToken1 = controller.get_userToken();
    int userToken2 = controller.get_userToken();
    int userToken3 = controller.get_userToken();
    Board board1 = controller.join_game(1, "Bob", userToken1);
    Board board2 = controller.create_game("Alice", userToken2);
    controller.join_game(board1.get_id(), "Charlie", userToken3);

    // Try joining again with the same user
    EXPECT_THROW(controller.join_game(board1.get_id(), "Bob", userToken1), std::invalid_argument);

    // Try joining the second game with the same user
    EXPECT_THROW(controller.join_game(board2.get_id(), "Charlie", userToken3), std::invalid_argument);
}

// Test the quit_game function
TEST(ControllerTest, QuitGameTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Quit the game with the user
    controller.quit_game(board.get_id(), userToken);

    // Ensure the user is no longer part of the game
    EXPECT_EQ(0, controller.get_games().size());
}

// Test if quit_game does not throw an exception if user not in the game 
TEST(ControllerTest, QuitGameInvalidTest) {
    Controller controller;
    // Create a game and join with user 1
    int userToken1 = controller.get_userToken();
    int userToken2 = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken1);

    // Quit the game with user 2
    EXPECT_NO_THROW(controller.quit_game(board.get_id(), userToken2));
}

// Test the start_game function
TEST(ControllerTest, StartGameTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Start the game
    controller.start_game(board.get_id(), userToken);

    // Ensure the game status is set to "Spielen"
    EXPECT_EQ(BoardStatus::Spielen, board.get_status());
}

// Test start_game exceptions
TEST(ControllerTest, StartGameInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Start the game
    controller.start_game(board.get_id(), userToken);

    // Try starting the game again, it should throw an exception
    EXPECT_THROW(controller.start_game(board.get_id(), userToken), std::invalid_argument);

    // Try starting the game with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.start_game(board.get_id(), anotherUserToken), std::invalid_argument);
}

// Test stop_game function
TEST(ControllerTest, StopGameTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);
    
    // Start the game
    controller.start_game(board.get_id(), userToken);
    controller.create_category(board.get_id(), userToken, "Animals");
    EXPECT_EQ(1, board.get_categories().size());

    controller.submit_category(board.get_id(), userToken, "Animals", "Tiger");

    // Stop the game
    controller.stop_game(board.get_id(), userToken);

    // Ensure the game status is set to "Bewerten"
    EXPECT_EQ(BoardStatus::Bewerten, board.get_status());
}

// Test stop_game exceptions
TEST(ControllerTest, StopGameInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Stop the game without starting it, it should throw an exception
    EXPECT_THROW(controller.stop_game(board.get_id(), userToken), std::invalid_argument);

    // Start the game
    controller.start_game(board.get_id(), userToken);

    // Stop the game with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.stop_game(board.get_id(), anotherUserToken), std::invalid_argument);

    // Stop the game without filling all categories, it should throw an exception
    EXPECT_THROW(controller.stop_game(board.get_id(), userToken), std::invalid_argument);
}

// Test get_game exception
TEST(ControllerTest, GetGameInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Try getting the game with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.get_game(board.get_id(), anotherUserToken), std::invalid_argument);
}

// Test create_category function
TEST(ControllerTest, CreateCategoryTest) {
    Controller controller;
    // Create a game with 0 categories
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);
    EXPECT_EQ(0, board.get_categories().size());

    // Create a new category
    controller.create_category(board.get_id(), userToken, "Animals");

    // Ensure the category is created
    EXPECT_EQ(1, board.get_categories().size());
}

// Test create_category exceptions
TEST(ControllerTest, CreateCategoryInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Try creating a category with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.create_category(board.get_id(), anotherUserToken, "Animals"), std::invalid_argument);

    // Create a category
    controller.create_category(board.get_id(), userToken, "Animals");

    // Try creating the same category again, it should throw an exception
    EXPECT_THROW(controller.create_category(board.get_id(), userToken, "Animals"), std::invalid_argument);
}

// Test delete_category function
TEST(ControllerTest, DeleteCategoryTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Create a new category
    controller.create_category(board.get_id(), userToken, "Animals");
    // board.add_category("Animals");
    EXPECT_EQ(1, board.get_categories().size());

    // Delete the category
    controller.delete_category(board.get_id(), userToken, "Animals");
    // board.remove_category("Animals");
    
    // Ensure the category is deleted
    EXPECT_EQ(0, board.get_categories().size());
}

// Test delete_category exceptions
TEST(ControllerTest, DeleteCategoryInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Try deleting a category with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.delete_category(board.get_id(), anotherUserToken, "Animals"), std::invalid_argument);

    // Try deleting a category that doesn't exist, it should not throw an exception
    EXPECT_NO_THROW(controller.delete_category(board.get_id(), userToken, "NonExistentCategory"));
}

// Test submit_category funciton
TEST(ControllerTest, SubmitCategoryTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Create a category and submit a value for it
    board = controller.create_category(board.get_id(), userToken, "Animals");

    // Start the game
    board = controller.start_game(board.get_id(), userToken);

    char letter = board.get_currentLetter();

    controller.submit_category(board.get_id(), userToken, "Animals", std::to_string(letter));

    board = controller.get_game(board.get_id(), userToken);

    

    // TODO: Ensure the value is correctly added to the board data
    // auto x = board.get_data();
}

// Test submit_category exceptions
TEST(ControllerTest, SubmitCategoryInvalidTest) {
    Controller controller;
    // Create a game and join with a user
    int userToken = controller.get_userToken();
    Board board = controller.create_game("Alice", userToken);

    // Try submitting a category value without starting the game, it should throw an exception
    EXPECT_THROW(controller.submit_category(board.get_id(), userToken, "Animals", "Elephant"), std::invalid_argument);

    // Start the game
    controller.start_game(board.get_id(), userToken);

    // Try submitting a category value with a different user, it should throw an exception
    int anotherUserToken = controller.get_userToken();
    EXPECT_THROW(controller.submit_category(board.get_id(), anotherUserToken, "Animals", "Elephant"), std::invalid_argument);

    // Try submitting a category value for a non-existent category, it should throw an exception
    EXPECT_THROW(controller.submit_category(board.get_id(), userToken, "NonexistentCategory", "Value"), std::invalid_argument);
}