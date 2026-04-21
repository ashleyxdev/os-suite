#!/usr/bin/env bash

echo "================================================"
echo "        Operating System Lab"
echo "        Lab File Downloader"
echo "================================================"
echo ""

# Base URL of your GitHub repo (raw content)
REPO_BASE="https://raw.githubusercontent.com/ashleyxdev/os-suite/main"

# Practicals definition
# Format: ID|FOLDER|DESCRIPTION|FILE1,FILE2,...
PRACTICALS=(
    "1|practical-1|Library Management (Shell Script)|library.sh"
    "2|practical-2.1|Fork, Zombie & Orphan Processes|fork-wait-demo.c,orphan-demo.c,zombie-demo.c"
    "3|practical-2.2|Process Scheduling (FCFS, SJF, RR, Priority)|main.c,search.c"
    "4|practical-3|Producer-Consumer Problem|producer-consumer.c"
    "5|practical-4|Reader-Writer Problem|reader-writer.c"
    "6|practical-5|Dining Philosophers Problem|dining_philosophers.c"
    "7|practical-6.1|IPC using Pipes|pipes.c,test.txt"
    "8|practical-6.2|IPC using Shared Memory|process1.c,process2.c"
)

# Display menu
echo "Available Practicals:"
echo "---------------------"
for entry in "${PRACTICALS[@]}"; do
    IFS='|' read -r id folder desc files <<< "$entry"
    file_list="${files//,/, }"
    echo "  $id. $desc"
    echo "     [$folder] $file_list"
done

# Build valid IDs string
VALID_IDS=""
for entry in "${PRACTICALS[@]}"; do
    IFS='|' read -r id _ _ _ <<< "$entry"
    if [ -n "$VALID_IDS" ]; then
        VALID_IDS="$VALID_IDS, $id"
    else
        VALID_IDS="$id"
    fi
done

echo ""
read -rp "Enter your choice [$VALID_IDS]: " CHOICE

# Find the selected entry
SELECTED=""
for entry in "${PRACTICALS[@]}"; do
    IFS='|' read -r id folder desc files <<< "$entry"
    if [[ "$id" == "$CHOICE" ]]; then
        SELECTED="$entry"
        break
    fi
done

if [ -z "$SELECTED" ]; then
    echo "Invalid choice! Please enter one of: $VALID_IDS"
    exit 1
fi

# Parse selected entry
IFS='|' read -r _ SEL_FOLDER SEL_DESC SEL_FILES <<< "$SELECTED"

# Create the practical folder locally
mkdir -p "$SEL_FOLDER"

echo ""
echo "Downloading $SEL_FOLDER..."

FAILED=0

# Download each source file
IFS=',' read -ra FILE_ARRAY <<< "$SEL_FILES"
for file in "${FILE_ARRAY[@]}"; do
    FILE_URL="$REPO_BASE/$SEL_FOLDER/$file"
    OUT_PATH="$SEL_FOLDER/$file"
    if curl -fsSL "$FILE_URL" -o "$OUT_PATH"; then
        echo "  ✅ $file"
    else
        echo "  ❌ Failed: $file"
        FAILED=1
    fi
done

if [ "$FAILED" -eq 1 ]; then
    echo ""
    echo "⚠️  Some files failed to download. Check the repository link."
else
    echo ""
    echo "✅ All files downloaded to ./$SEL_FOLDER/"
fi

echo ""
echo "Done!"