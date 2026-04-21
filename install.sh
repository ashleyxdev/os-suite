#!/usr/bin/env bash

echo "================================================"
echo "        Cloud Computing Lab"
echo "        Lab File Downloader"
echo "================================================"
echo ""

# Base URL of your GitHub repo (raw content)
REPO_BASE="https://raw.githubusercontent.com/ashleyxdev/ccl-suite/main"

# Parallel arrays: ids, names, types, sections
IDS=(1 4 6 7 8 9 10 99 100)

NAMES=(
    "1-linux-cmds.md"
    "4-ec2-server.md"
    "6-s3-bucket.md"
    "7-terraform.md"
    "8-ansible.md"
    "9-docker.md"
    "10-file-transfer.md"
    "index.html"
    "hello-world-key.pem"
)

TYPES=(
    "Markdown"
    "Markdown"
    "Markdown"
    "Markdown"
    "Markdown"
    "Markdown"
    "Markdown"
    "HTML"
    "PEM"
)

SECTIONS=(
    "Practical"
    "Practical"
    "Practical"
    "Practical"
    "Practical"
    "Practical"
    "Practical"
    "Resource"
    "Resource"
)

# Display: Lab Practicals
echo "Lab Practicals:"
echo "---------------"
for i in "${!IDS[@]}"; do
    if [[ "${SECTIONS[$i]}" == "Practical" ]]; then
        echo "  ${IDS[$i]}. [${TYPES[$i]}] ${NAMES[$i]}"
    fi
done

echo ""

# Display: Resource Files
echo "Resource Files:"
echo "---------------"
for i in "${!IDS[@]}"; do
    if [[ "${SECTIONS[$i]}" == "Resource" ]]; then
        echo "  ${IDS[$i]}. [${TYPES[$i]}] ${NAMES[$i]}"
    fi
done

# Build valid IDs string for display
VALID_IDS=$(IFS=", "; echo "${IDS[*]}")

echo ""
read -rp "Enter your choice [$VALID_IDS]: " CHOICE

# Find the selected entry
SELECTED_INDEX=-1
for i in "${!IDS[@]}"; do
    if [[ "${IDS[$i]}" == "$CHOICE" ]]; then
        SELECTED_INDEX=$i
        break
    fi
done

if [[ $SELECTED_INDEX -eq -1 ]]; then
    echo "Invalid choice! Please enter one of: $VALID_IDS"
    exit 1
fi

SELECTED_NAME="${NAMES[$SELECTED_INDEX]}"
FILE_URL="$REPO_BASE/$SELECTED_NAME"

echo ""
echo "Downloading $SELECTED_NAME..."

if curl -fsSL "$FILE_URL" -o "$SELECTED_NAME"; then
    echo "✅ Download complete! File saved as: $SELECTED_NAME"
else
    echo "❌ Failed to download file. Please check the repository link or filename."
    exit 1
fi

echo ""
echo "Done!"