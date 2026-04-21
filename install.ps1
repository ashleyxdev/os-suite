Write-Host "================================================"
Write-Host "        Operating System Lab"
Write-Host "        Lab File Downloader"
Write-Host "================================================"
Write-Host ""

# Base URL of your GitHub repo (raw content)
$repoBase = "https://raw.githubusercontent.com/ashleyxdev/os-suite/main"

# Practicals: each entry has an id, folder name, description, and list of source files
$practicals = @(
    @{ id = 1;   folder = "practical-1";   desc = "Library Management (Shell Script)";
       files = @("library.sh") },

    @{ id = 2;   folder = "practical-2.1"; desc = "Fork, Zombie & Orphan Processes";
       files = @("fork-wait-demo.c", "orphan-demo.c", "zombie-demo.c") },

    @{ id = 3;   folder = "practical-2.2"; desc = "Process Scheduling (FCFS, SJF, RR, Priority)";
       files = @("main.c", "search.c") },

    @{ id = 4;   folder = "practical-3";   desc = "Producer-Consumer Problem";
       files = @("producer-consumer.c") },

    @{ id = 5;   folder = "practical-4";   desc = "Reader-Writer Problem";
       files = @("reader-writer.c") },

    @{ id = 6;   folder = "practical-5";   desc = "Dining Philosophers Problem";
       files = @("dining_philosophers.c") },

    @{ id = 7;   folder = "practical-6.1"; desc = "IPC using Pipes";
       files = @("pipes.c", "test.txt") },

    @{ id = 8;   folder = "practical-6.2"; desc = "IPC using Shared Memory";
       files = @("process1.c", "process2.c") }
)

# Resource files (downloaded to current directory, not into a folder)
$resources = @(
    @{ id = 99;  name = "SETUP.md"; desc = "WSL & Build Tools Setup Guide" }
)

# Display menu
Write-Host "Available Practicals:"
Write-Host "---------------------"
foreach ($p in $practicals) {
    $fileList = $p.files -join ", "
    Write-Host "  $($p.id). $($p.desc)"
    Write-Host "     [$($p.folder)] $fileList"
}

Write-Host ""
Write-Host "Resource Files:"
Write-Host "---------------"
foreach ($r in $resources) {
    Write-Host "  $($r.id). $($r.desc)  [$($r.name)]"
}

# Prompt for choice
$allIds = ($practicals | ForEach-Object { $_.id }) + ($resources | ForEach-Object { $_.id })
$validIds = $allIds -join ", "
Write-Host ""
$choice = Read-Host "Enter your choice [$validIds]"

# Check if it's a resource file
$selectedResource = $resources | Where-Object { $_.id -eq [int]$choice }
if ($selectedResource) {
    $fileUrl = "$repoBase/$($selectedResource.name)"
    Write-Host "`nDownloading $($selectedResource.name)..."
    try {
        Invoke-WebRequest -Uri $fileUrl -OutFile $selectedResource.name -ErrorAction Stop
        Write-Host "✅ Download complete! File saved as: $($selectedResource.name)"
    } catch {
        Write-Host "❌ Failed to download file."
    }
    Write-Host "`nDone!"
    exit
}

$selected = $practicals | Where-Object { $_.id -eq [int]$choice }

if (-not $selected) {
    Write-Host "Invalid choice! Please enter one of: $validIds"
    exit
}

# Create the practical folder locally
$folder = $selected.folder
if (-not (Test-Path $folder)) {
    New-Item -ItemType Directory -Path $folder | Out-Null
}

# Download each source file into the folder
Write-Host "`nDownloading $folder..."
$failed = $false

foreach ($file in $selected.files) {
    $fileUrl = "$repoBase/$folder/$file"
    $outPath = Join-Path $folder $file
    try {
        Invoke-WebRequest -Uri $fileUrl -OutFile $outPath -ErrorAction Stop
        Write-Host "  ✅ $file"
    } catch {
        Write-Host "  ❌ Failed: $file"
        $failed = $true
    }
}

if ($failed) {
    Write-Host "`n⚠️  Some files failed to download. Check the repository link."
} else {
    Write-Host "`n✅ All files downloaded to ./$folder/"
}

Write-Host "`nDone!"