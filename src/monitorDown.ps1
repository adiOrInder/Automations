$folder = "C:\Users\all\Downloads"
$filter = "*.*"
$trigger = New-ScheduledTaskTrigger -Once -At (Get-Date).AddMinutes(1) -RepetitionInterval (New-TimeSpan -Minutes 1) -RepetitionDuration ([timespan]::MaxValue)
$action = New-ScheduledTaskAction -Execute "powershell.exe" -Argument "-ExecutionPolicy Bypass -File 'C:/.dotfiles/Automations/src/monitorDownloads.ps1'"

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
Register-ScheduledJob -Name "MonitorDownloads" -Trigger $trigger -Action $action
