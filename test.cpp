#include <gtest/gtest.h>
#include <vector>
#include <map>
#include <set>
#include <sqlite3.h>
#include <string>

// Symbolic constants for better readability and maintainability
const int INITIAL_CAPACITY = 100;
const int RESIZE_SMALLER = 5;
const int RESIZE_LARGER = 10;
const int RESERVE_CAPACITY = 50;
const int OUT_OF_RANGE_INDEX = 10;
const int PUSH_BACK_VALUE = 42;
const int NUM_VALUES = 5;

// Test suite for collection operations
class CollectionTest : public ::testing::Test {
protected:
    std::vector<int> vec;

    void SetUp() override {
        // Initial setup before each test
    }

    void TearDown() override {
        // Cleanup after each test
    }
};

// Test to ensure the vector is initialized empty
TEST_F(CollectionTest, VectorInitiallyEmpty) {
    EXPECT_TRUE(vec.empty()); // The vector should be empty initially
}

// Test to add a single value to an empty collection
TEST_F(CollectionTest, AddSingleValue) {
    vec.push_back(1);
    EXPECT_EQ(vec.size(), 1); // The size should be 1 after adding one element
    EXPECT_EQ(vec[0], 1); // The first element should be 1
}

// Test to add five values to the collection
TEST_F(CollectionTest, AddFiveValues) {
    for (int i = 1; i <= NUM_VALUES; ++i) {
        vec.push_back(i);
    }
    EXPECT_EQ(vec.size(), NUM_VALUES); // The size should be 5
    for (int i = 0; i < NUM_VALUES; ++i) {
        EXPECT_EQ(vec[i], i + 1); // The elements should be 1, 2, 3, 4, 5
    }
}

// Test to check maximum size and capacity
TEST_F(CollectionTest, MaxSizeAndCapacity) {
    vec.reserve(INITIAL_CAPACITY);
    EXPECT_GE(vec.capacity(), INITIAL_CAPACITY); // The capacity should be at least 100
}

// Test to resize the collection
TEST_F(CollectionTest, ResizeCollection) {
    vec.resize(RESIZE_LARGER);
    EXPECT_EQ(vec.size(), RESIZE_LARGER); // The size should be 10 after resizing
    vec.resize(RESIZE_SMALLER);
    EXPECT_EQ(vec.size(), RESIZE_SMALLER); // The size should be 5 after resizing
    vec.resize(0);
    EXPECT_TRUE(vec.empty()); // The vector should be empty after resizing to 0
}

// Test to clear the collection
TEST_F(CollectionTest, ClearCollection) {
    vec.push_back(1);
    vec.clear();
    EXPECT_TRUE(vec.empty()); // The vector should be empty after clearing
}

// Test to erase all elements
TEST_F(CollectionTest, EraseAllElements) {
    vec = {1, 2, 3, 4, 5};
    vec.erase(vec.begin(), vec.end());
    EXPECT_TRUE(vec.empty()); // The vector should be empty after erasing all elements
}

// Test to reserve capacity
TEST_F(CollectionTest, ReserveCapacity) {
    vec.reserve(RESERVE_CAPACITY);
    EXPECT_GE(vec.capacity(), RESERVE_CAPACITY); // The capacity should be at least 50
}

// Test to access out of range
TEST_F(CollectionTest, AccessOutOfRange) {
    EXPECT_THROW(vec.at(OUT_OF_RANGE_INDEX), std::out_of_range); // Accessing index 10 should throw an exception
}

// Custom test to verify pushing back increases size
TEST_F(CollectionTest, PushBackIncreasesSize) {
    auto initialSize = vec.size();
    vec.push_back(PUSH_BACK_VALUE);
    EXPECT_EQ(vec.size(), initialSize + 1); // The size should increase by 1 after push_back
}

// Custom test to shrink to smaller than size
TEST_F(CollectionTest, ShrinkToSmallerThanSize) {
    vec = {1, 2, 3, 4, 5};
    vec.resize(2);
    EXPECT_EQ(vec.size(), 2); // The size should be 2 after resizing
    EXPECT_THROW(vec.at(4), std::out_of_range); // Accessing index 4 should throw an exception
}

// Additional edge case: inserting at specific positions
TEST_F(CollectionTest, InsertAtSpecificPosition) {
    vec = {1, 2, 4, 5};
    vec.insert(vec.begin() + 2, 3); // Insert 3 at the 3rd position
    EXPECT_EQ(vec[2], 3); // The element at index 2 should be 3
    EXPECT_EQ(vec.size(), 5); // The size should be 5 after insertion
}

// Additional edge case: removing specific elements
TEST_F(CollectionTest, RemoveSpecificElement) {
    vec = {1, 2, 3, 4, 5};
    vec.erase(vec.begin() + 2); // Remove the element at the 3rd position
    EXPECT_EQ(vec.size(), 4); // The size should be 4 after removal
    EXPECT_EQ(vec[2], 4); // The element at index 2 should now be 4
}

// Use more complex data structures
TEST_F(CollectionTest, UseMap) {
    std::map<int, int> myMap;
    myMap[1] = 10;
    EXPECT_EQ(myMap[1], 10);
}

TEST_F(CollectionTest, UseSet) {
    std::set<int> mySet;
    mySet.insert(1);
    EXPECT_TRUE(mySet.find(1) != mySet.end());
}

// Optimize Algorithms for Better Performance with Increased Scope
TEST_F(CollectionTest, PerformanceTestAddElements) {
    for (int i = 0; i < 1000000; ++i) {
        vec.push_back(i);
    }
    EXPECT_EQ(vec.size(), 1000000);
}

// Test suite for database operations using SQLite
class DatabaseTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        int rc = sqlite3_open(":memory:", &db);
        ASSERT_EQ(rc, SQLITE_OK) << "Cannot open database: " << sqlite3_errmsg(db);
        const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT);";
        rc = sqlite3_exec(db, sql, 0, 0, 0);
        ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << sqlite3_errmsg(db);
    }

    void TearDown() override {
        sqlite3_close(db);
    }

    void insertData(int id, const std::string &value) {
        const char *sql = "INSERT INTO test (id, value) VALUES (?, ?);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    std::string readData(int id) {
        const char *sql = "SELECT value FROM test WHERE id = ?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, id);
        std::string value;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
        return value;
    }

    void updateData(int id, const std::string &value) {
        const char *sql = "UPDATE test SET value = ? WHERE id = ?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    void deleteData(int id) {
        const char *sql = "DELETE FROM test WHERE id = ?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
};

// Test to insert data into the database
TEST_F(DatabaseTest, InsertData) {
    insertData(1, "test_value");
    std::string value = readData(1);
    EXPECT_EQ(value, "test_value");
}

// Test to read data from the database
TEST_F(DatabaseTest, ReadData) {
    insertData(1, "test_value");
    std::string value = readData(1);
    EXPECT_EQ(value, "test_value");
}

// Test to update data in the database
TEST_F(DatabaseTest, UpdateData) {
    insertData(1, "initial_value");
    updateData(1, "updated_value");
    std::string value = readData(1);
    EXPECT_EQ(value, "updated_value");
}

// Test to delete data from the database
TEST_F(DatabaseTest, DeleteData) {
    insertData(1, "test_value");
    deleteData(1);
    std::string value = readData(1);
    EXPECT_EQ(value, ""); // No data should be found after deletion
}
