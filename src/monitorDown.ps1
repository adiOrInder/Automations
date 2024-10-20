$folder = "C:\Users\all\Downloads"
$filter = "*.*"

$fsw = New-Object IO.FileSystemWatcher $folder, $filter
$fsw.IncludeSubdirectories = $false
$fsw.EnableRaisingEvents = $true

$action = {
    $name = $Event.SourceEventArgs.Name
    Write-Host "File '$name' has been created in $folder"

    gcc "C:/.dotfiles/Automations/main.c"
    .\a.exe
}

Register-ObjectEvent $fsw Created -Action $action
Register-ObjectEvent $fsw Changed -Action $action

while ($true) {
    Start-Sleep -Seconds 10
}
