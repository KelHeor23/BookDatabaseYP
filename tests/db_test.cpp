#include "book.hpp"
#include "book_database.hpp"

#include <gtest/gtest.h>
#include "book_database.hpp"

TEST(BookDatabaseTest, BookDatabasePushBack) {
    bookdb::BookDatabase<> db;
    bookdb::Book b1{"1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190};
    bookdb::Book b2{"Animal Farm", "George Orwell", 1945, bookdb::Genre::Fiction, 4.4, 143};

    db.PushBack(b1);
    db.PushBack(b2);

    EXPECT_EQ(db.size(), 2u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(db.GetBooks()[0].author, author);
    EXPECT_EQ(db.GetBooks()[1].author, author);
}

TEST(BookDatabaseTest, BookDatabaseInitList) {
    bookdb::Book b1{"1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190};
    bookdb::Book b2{"Animal Farm", "George Orwell", 1945, bookdb::Genre::Fiction, 4.4, 143};

    bookdb::BookDatabase<> db{ b1, b2 };

    EXPECT_EQ(db.size(), 2u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(db.GetBooks()[0].author, author);
    EXPECT_EQ(db.GetBooks()[1].author, author);
}

TEST(BookDatabaseTest, BookDatabaseEmplaceBack) {
    bookdb::BookDatabase<> db;

    auto& b = db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190);

    EXPECT_EQ(db.size(), 1u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(b.author, author);
    EXPECT_EQ(db.GetBooks()[0].author, author);
}