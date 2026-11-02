#!/bin/bash

echo "[+] Cloning and setting up agent_1..."

# کامپایل مستقیم سورس‌کد با C11
gcc agent_1.c -std=c11 -O3 -o agent_1

if [ $? -eq 0 ]; then
    echo "[+] agent_1 successfully compiled!"
    echo "[*] To run the agent, type: ./agent_1"
    echo "[*] To queue an update task, put your command in task1.txt and type 'runtask' inside the agent."
else
    echo "[-] Compilation failed. Please check if gcc and build-essential are installed."
    exit 1
fi
