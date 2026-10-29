# Script otomatis untuk menambal library PS3_Controller_Host agar kompatibel dengan Arduino ESP32 Core 3.x
param(
    [string]$LibraryPath = "$HOME\Documents\Arduino\libraries\PS3_Controller_Host\src"
)

if (-not (Test-Path $LibraryPath)) {
    Write-Error "Folder library tidak ditemukan di: $LibraryPath. Pastikan library 'PS3 Controller Host' sudah diinstal di Arduino IDE."
    exit 1
}

Copy-Item -Path "$PSScriptRoot\ps3.c" -Destination "$LibraryPath\ps3.c" -Force
Copy-Item -Path "$PSScriptRoot\Ps3Controller.cpp" -Destination "$LibraryPath\Ps3Controller.cpp" -Force

Write-Host "[OK] Patch ESP32 Core 3.x berhasil diterapkan ke $LibraryPath" -ForegroundColor Green
