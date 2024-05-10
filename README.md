
# RDP-hook-stealer
This is a tool that I code in my process of understanding windows internals. I thought it was a very good idea to deploy it on jump servers. The result in a production environment was really impressive.


## Files
There are two files: rdp_indisk.cpp and rdp_inmemory.cpp. They are quite self-descriptive, but basically one injects the DLL from disk, and another from memory, being in shellcode format in the binary itself

## Compilation
To compile the indisk:
``x86_64-w64-mingw32-gcc rdp_indisk.cpp -o rdp_indisk.exe -lstdc++ -static``

To compile the inmemory
``x86_64-w64-mingw32-gcc rdp_inmemory.cpp -o rdp_inmemory.exe -lstdc++ -static``

## Example
![image](https://github.com/juanga333/RDP-hook-stealer/assets/51821633/24ada562-c215-4ca8-9bc6-6182e86dffa2)


## TODO
- Encrypt shellcode loaded into memory
- Support for sending shellcode via socket (network)

