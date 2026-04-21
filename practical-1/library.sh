#!/bin/bash

# The database file — this is our "table"
DB="library.db"

# Create the file if it doesn't already exist
if [ ! -f "$DB" ]; then
    touch "$DB"
fi

# ─── Display all books ────────────────────────────────────────────
display_books() {
    echo ""
    echo "=========================================="
    echo "        LIBRARY DATABASE"
    echo "=========================================="
    printf "%-8s %-25s %-20s %-12s\n" "BookID" "Title" "Author" "Status"
    echo "------------------------------------------"

    if [ ! -s "$DB" ]; then
        echo "  No books in the database."
    else
        while IFS='|' read -r id title author status; do
            printf "%-8s %-25s %-20s %-12s\n" "$id" "$title" "$author" "$status"
        done < "$DB"
    fi
    echo "=========================================="
}

# ─── Add a book ───────────────────────────────────────────────────
add_book() {
    echo ""
    read -p "Enter Book ID   : " id
    read -p "Enter Title     : " title
    read -p "Enter Author    : " author

    # Check if ID already exists
    if grep -q "^$id|" "$DB"; then
        echo "ERROR: Book ID '$id' already exists!"
        return
    fi

    echo "$id|$title|$author|Available" >> "$DB"
    echo "Book '$title' added successfully!"
}

# ─── Search a book ────────────────────────────────────────────────
search_book() {
    echo ""
    read -p "Enter title or author to search: " keyword

    result=$(grep -i "$keyword" "$DB")

    if [ -z "$result" ]; then
        echo "No book found matching '$keyword'."
    else
        echo ""
        printf "%-8s %-25s %-20s %-12s\n" "BookID" "Title" "Author" "Status"
        echo "------------------------------------------"
        echo "$result" | while IFS='|' read -r id title author status; do
            printf "%-8s %-25s %-20s %-12s\n" "$id" "$title" "$author" "$status"
        done
    fi
}

# ─── Delete a book ────────────────────────────────────────────────
delete_book() {
    echo ""
    read -p "Enter Book ID to delete: " id

    if ! grep -q "^$id|" "$DB"; then
        echo "ERROR: Book ID '$id' not found."
        return
    fi

    # grep -v keeps all lines EXCEPT the matching one
    grep -v "^$id|" "$DB" > temp.db && mv temp.db "$DB"
    echo "Book with ID '$id' deleted successfully."
}

# ─── Issue a book ─────────────────────────────────────────────────
issue_book() {
    echo ""
    read -p "Enter Book ID to issue: " id

    if ! grep -q "^$id|" "$DB"; then
        echo "ERROR: Book ID '$id' not found."
        return
    fi

    # Check current status
    status=$(grep "^$id|" "$DB" | awk -F'|' '{print $4}')

    if [ "$status" = "Issued" ]; then
        echo "Book is already issued."
        return
    fi

    # Replace 'Available' with 'Issued' for that specific line
    sed -i "s/^$id|\(.*\)|Available/$id|\1|Issued/" "$DB"
    echo "Book '$id' issued successfully."
}

# ─── Return a book ────────────────────────────────────────────────
return_book() {
    echo ""
    read -p "Enter Book ID to return: " id

    if ! grep -q "^$id|" "$DB"; then
        echo "ERROR: Book ID '$id' not found."
        return
    fi

    status=$(grep "^$id|" "$DB" | awk -F'|' '{print $4}')

    if [ "$status" = "Available" ]; then
        echo "Book is not issued. Cannot return."
        return
    fi

    sed -i "s/^$id|\(.*\)|Issued/$id|\1|Available/" "$DB"
    echo "Book '$id' returned successfully."
}

# ─── Main Menu Loop ───────────────────────────────────────────────
while true; do
    echo ""
    echo "======= LIBRARY MANAGEMENT SYSTEM ======="
    echo "  1. Display All Books"
    echo "  2. Add a Book"
    echo "  3. Search a Book"
    echo "  4. Delete a Book"
    echo "  5. Issue a Book"
    echo "  6. Return a Book"
    echo "  7. Exit"
    echo "=========================================="
    read -p "Enter your choice [1-7]: " choice

    case $choice in
        1) display_books ;;
        2) add_book ;;
        3) search_book ;;
        4) delete_book ;;
        5) issue_book ;;
        6) return_book ;;
        7) echo "Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please enter 1-7." ;;
    esac
done
