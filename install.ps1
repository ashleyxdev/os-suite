Write-Host "================================================"
Write-Host "        Cloud Computing Lab"
Write-Host "        Lab File Downloader"
Write-Host "================================================"
Write-Host ""

# Base URL of your GitHub repo (raw content)
$repoBase = "https://raw.githubusercontent.com/ashleyxdev/ccl-suite/main"

# ID for practicals = their filename prefix number
# ID for resource files = 99, 100, ... (to avoid conflict with future practicals)
$labFiles = @(
    @{ id = 1;   name = "1-linux-cmds.md";      type = "Markdown"; section = "Practical" },
    @{ id = 4;   name = "4-ec2-server.md";      type = "Markdown"; section = "Practical" },
    @{ id = 6;   name = "6-s3-bucket.md";       type = "Markdown"; section = "Practical" },
    @{ id = 7;   name = "7-terraform.md";       type = "Markdown"; section = "Practical" },
    @{ id = 8;   name = "8-ansible.md";         type = "Markdown"; section = "Practical" },
    @{ id = 9;   name = "9-docker.md";          type = "Markdown"; section = "Practical" },
    @{ id = 10;  name = "10-file-transfer.md";  type = "Markdown"; section = "Practical" },
    @{ id = 99;  name = "index.html";           type = "HTML";     section = "Resource"  },
    @{ id = 100; name = "hello-world-key.pem";  type = "PEM";      section = "Resource"  }
)

# Display: Lab Practicals
Write-Host "Lab Practicals:"
Write-Host "---------------"
foreach ($f in $labFiles | Where-Object { $_.section -eq "Practical" }) {
    Write-Host "  $($f.id). [$($f.type)] $($f.name)"
}

Write-Host ""

# Display: Resource Files
Write-Host "Resource Files:"
Write-Host "---------------"
foreach ($f in $labFiles | Where-Object { $_.section -eq "Resource" }) {
    Write-Host "  $($f.id). [$($f.type)] $($f.name)"
}

# Show valid choices explicitly
$validIds = ($labFiles | ForEach-Object { $_.id }) -join ", "
Write-Host ""
$choice = Read-Host "Enter your choice [$validIds]"

$selected = $labFiles | Where-Object { $_.id -eq [int]$choice }

if (-not $selected) {
    Write-Host "Invalid choice! Please enter one of: $validIds"
    exit
}

# Download selected file
$fileUrl = "$repoBase/$($selected.name)"
Write-Host "`nDownloading $($selected.name)..."
try {
    Invoke-WebRequest -Uri $fileUrl -OutFile $selected.name -ErrorAction Stop
    Write-Host "✅ Download complete! File saved as: $($selected.name)"
} catch {
    Write-Host "❌ Failed to download file. Please check the repository link or filename."
}

Write-Host "`nDone!"