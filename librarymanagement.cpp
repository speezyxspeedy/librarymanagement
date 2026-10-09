from datetime import datetime


class Book:
    def __init__(self, book_id: str, title: str, author: str, total_copies: int):
        self.book_id = book_id
        self.title = title
        self.author = author
        self.total_copies = total_copies
        self.available_copies = total_copies

    def __str__(self):
        return f"[{self.book_id}] '{self.title}' by {self.author} | Available: {self.available_copies}/{self.total_copies}"


class Member:
    def __init__(self, member_id: str, name: str):
        self.member_id = member_id
        self.name = name
        # book_id -> issue_date
        self.borrowed_books = {}

    def __str__(self):
        borrowed = list(self.borrowed_books.keys())
        return f"Member [{self.member_id}]: {self.name} | Borrowed Book IDs: {borrowed if borrowed else 'None'}"


class LibrarySystem:
    def __init__(self):
        self.books = {}    # book_id -> Book object
        self.members = {}  # member_id -> Member object

    # 1. Add Book
    def add_book(self, book_id: str, title: str, author: str, copies: int = 1):
        if book_id in self.books:
            self.books[book_id].total_copies += copies
            self.books[book_id].available_copies += copies
            print(f"Updated copies for book '{title}'. Total: {self.books[book_id].total_copies}")
        else:
            self.books[book_id] = Book(book_id, title, author, copies)
            print(f"Book '{title}' added successfully!")

    # 2. Register Member
    def register_member(self, member_id: str, name: str):
        if member_id in self.members:
            print(f"Member ID {member_id} already exists.")
        else:
            self.members[member_id] = Member(member_id, name)
            print(f"Member '{name}' registered successfully!")

    # 3. Issue Book
    def issue_book(self, book_id: str, member_id: str):
        if book_id not in self.books:
            print(f"Error: Book ID '{book_id}' not found.")
            return

        if member_id not in self.members:
            print(f"Error: Member ID '{member_id}' not found.")
            return

        book = self.books[book_id]
        member = self.members[member_id]

        if book_id in member.borrowed_books:
            print(f"Error: Member '{member.name}' already has a copy of this book.")
            return

        if book.available_copies <= 0:
            print(f"Sorry, '{book.title}' is currently out of stock.")
            return

        book.available_copies -= 1
        issue_time = datetime.now().strftime("%Y-%m-%d %H:%M")
        member.borrowed_books[book_id] = issue_time
        print(f"Success: '{book.title}' issued to {member.name} on {issue_time}.")

    # 4. Return Book
    def return_book(self, book_id: str, member_id: str):
        if book_id not in self.books or member_id not in self.members:
            print("Error: Invalid Book ID or Member ID.")
            return

        book = self.books[book_id]
        member = self.members[member_id]

        if book_id not in member.borrowed_books:
            print(f"Error: Member '{member.name}' does not have this book borrowed.")
            return

        del member.borrowed_books[book_id]
        book.available_copies += 1
        print(f"Success: '{book.title}' returned by {member.name}.")

    # 5. Search Functionality
    def search_books(self, query: str):
        query_lower = query.strip().lower()
        results = [
            b for b in self.books.values()
            if query_lower in b.title.lower() or query_lower in b.author.lower()
        ]

        if not results:
            print(f"No books found matching '{query}'.")
        else:
            print(f"\n--- Search Results for '{query}' ---")
            for b in results:
                print(b)
            print("---------------------------------")

    # 6. Display All
    def display_all_books(self):
        if not self.books:
            print("The library catalog is currently empty.")
            return
        print("\n=== Current Library Inventory ===")
        for book in self.books.values():
            print(book)
        print("==================================")


# CLI Menu Interface
def main():
    library = LibrarySystem()

    # Pre-populating sample data
    library.add_book("B101", "Clean Code", "Robert C. Martin", 3)
    library.add_book("B102", "The Pragmatic Programmer", "Andy Hunt", 2)
    library.register_member("M001", "Rahul Sharma")

    while True:
        print("\n===== LIBRARY MANAGEMENT SYSTEM =====")
        print("1. Add New Book")
        print("2. Register New Member")
        print("3. Issue Book")
        print("4. Return Book")
        print("5. Search Book (Title / Author)")
        print("6. Show All Books")
        print("7. Exit")
        choice = input("Enter your choice (1-7): ").strip()

        if choice == "1":
            b_id = input("Enter Book ID: ").strip()
            title = input("Enter Title: ").strip()
            author = input("Enter Author: ").strip()
            copies = int(input("Enter Number of Copies: ").strip() or "1")
            library.add_book(b_id, title, author, copies)

        elif choice == "2":
            m_id = input("Enter Member ID: ").strip()
            name = input("Enter Member Name: ").strip()
            library.register_member(m_id, name)

        elif choice == "3":
            b_id = input("Enter Book ID to issue: ").strip()
            m_id = input("Enter Member ID: ").strip()
            library.issue_book(b_id, m_id)

        elif choice == "4":
            b_id = input("Enter Book ID to return: ").strip()
            m_id = input("Enter Member ID: ").strip()
            library.return_book(b_id, m_id)

        elif choice == "5":
            query = input("Enter title or author name to search: ").strip()
            library.search_books(query)

        elif choice == "6":
            library.display_all_books()

        elif choice == "7":
            print("Exiting Library Management System. Goodbye!")
            break

        else:
            print("Invalid selection. Please choose an option between 1 and 7.")


if __name__ == "__main__":
    main()