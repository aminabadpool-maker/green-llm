#!/bin/bash
set -e
echo "[*] Building iWeb..."
gcc iweb.c -std=c11 -O3 -Wall -Wextra -o iweb
echo "[+] Done!"
