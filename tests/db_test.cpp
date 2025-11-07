#include "book.hpp"
#include "book_database.hpp"
#include "filters.hpp"
#include "comparators.hpp"
#include "statsistics.hpp"

#include <gtest/gtest.h>
#include "book_database.hpp"

using namespace bookdb;

BookDatabase<> makeDB() {
    BookDatabase<> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    return db;
}

TEST(BookDatabase, PushBack) {
    BookDatabase<> db;
    Book b1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    Book b2{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};

    db.PushBack(b1);
    db.PushBack(b2);

    EXPECT_EQ(db.size(), 2u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(db.GetBooks()[0].author, author);
    EXPECT_EQ(db.GetBooks()[1].author, author);
}

TEST(BookDatabase, InitList) {
    Book b1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    Book b2{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};

    BookDatabase<> db{ b1, b2 };

    EXPECT_EQ(db.size(), 2u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(db.GetBooks()[0].author, author);
    EXPECT_EQ(db.GetBooks()[1].author, author);
}

TEST(BookDatabase, EmplaceBack) {
    BookDatabase<> db;

    auto& b = db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);

    EXPECT_EQ(db.size(), 1u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);

    const auto& author = *db.GetAuthors().begin();
    EXPECT_EQ(b.author, author);
    EXPECT_EQ(db.GetBooks()[0].author, author);
}

TEST(BookStruct, GenreStringRoundtrip) {
    EXPECT_EQ(GenreFromString("Fiction"), Genre::Fiction);
    EXPECT_EQ(GenreFromString("NonFiction"), Genre::NonFiction);
    EXPECT_EQ(GenreFromString("SciFi"), Genre::SciFi);
    EXPECT_EQ(GenreFromString("Biography"), Genre::Biography);
    EXPECT_EQ(GenreFromString("Mystery"), Genre::Mystery);
    EXPECT_EQ(GenreFromString("Unknown"), Genre::Unknown);

    EXPECT_EQ(GenreToString(Genre::Fiction), "Fiction");
    EXPECT_EQ(GenreToString(Genre::NonFiction), "NonFiction");
    EXPECT_EQ(GenreToString(Genre::SciFi), "SciFi");
    EXPECT_EQ(GenreToString(Genre::Biography), "Biography");
    EXPECT_EQ(GenreToString(Genre::Mystery), "Mystery");
    EXPECT_EQ(GenreToString(Genre::Unknown), "Unknown");
}

TEST(FiltersPredicates, YearRatingGenreAndCombinators) {
    auto db = makeDB();
    auto year20th = YearBetween(1900, 1999);
    auto high = RatingAbove(4.5);
    auto scifi = GenreIs(Genre::SciFi);

    auto filtered = filterBooks(db.begin(), db.end(), all_of(year20th, high));
    for (const Book& b : db.GetBooks()) {
        bool in = year20th(b) && high(b);
        bool found = std::any_of(filtered.begin(), filtered.end(), [&](const auto& r){ return &r.get() == &b; });
        EXPECT_EQ(in, found);
    }

    auto any_res = filterBooks(db.begin(), db.end(), any_of(scifi, high));
    for (const Book& b : db.GetBooks()) {
        bool in = scifi(b) || high(b);
        bool found = std::any_of(any_res.begin(), any_res.end(), [&](const auto& r){ return &r.get() == &b; });
        EXPECT_EQ(in, found);
    }
}

TEST(Comparators, SortOrders) {
    auto db = makeDB();

    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    for (std::size_t i = 1; i < db.size(); ++i) {
        EXPECT_LE(db.GetBooks()[i-1].author, db.GetBooks()[i].author);
    }

    std::sort(db.begin(), db.end(), comp::MoreByRating{});
    for (std::size_t i = 1; i < db.size(); ++i) {
        EXPECT_GE(db.GetBooks()[i-1].rating, db.GetBooks()[i].rating);
    }

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});
    for (std::size_t i = 1; i < db.size(); ++i) {
        EXPECT_LE(db.GetBooks()[i-1].read_count, db.GetBooks()[i].read_count);
    }
}

TEST(Statistics, AverageRatingAndTopN) {
    BookDatabase<> db;
    db.EmplaceBack("A", "X", 2000, Genre::Fiction, 4.0, 10);
    db.EmplaceBack("B", "Y", 2001, Genre::SciFi,   3.0, 20);
    db.EmplaceBack("C", "Z", 2002, Genre::SciFi,   5.0, 30);

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), (4.0 + 3.0 + 5.0) / 3.0);

    auto top2 = getTopNBy(db, 2, comp::MoreByRating{});
    ASSERT_EQ(top2.size(), 2u);
    std::vector<double> ratings{ top2[0].get().rating, top2[1].get().rating };
    std::sort(ratings.begin(), ratings.end(), std::greater<>());
    EXPECT_EQ(ratings[0], 5.0);
    EXPECT_EQ(ratings[1], 4.0);
}

TEST(Statistics, GenreRatingsAndAuthorHistogram) {
    auto db = makeDB();

    auto genreText = calculateGenreRatings(db);
    EXPECT_NE(genreText.find("Fiction:"), std::string::npos);
    EXPECT_NE(genreText.find("SciFi:"), std::string::npos);

    auto hist = buildAuthorHistogramFlat(db);
    EXPECT_NE(hist.find("George Orwell:"), std::string::npos);
    EXPECT_NE(hist.find("J.R.R. Tolkien:"), std::string::npos);
}
