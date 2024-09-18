#ifndef UTILS
#define UTILS

#include <unistd.h>
#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <dlfcn.h>
#include "Logger.h"

bool isUnity = false;
bool libLoaded = false;

typedef uintptr_t ADDRESS;

typedef uint64_t QWORD;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef float FLOAT;
typedef double DOUBLE;

struct AddressMap
{
    ADDRESS startAddress;
    ADDRESS endAddress;
};

namespace Utils
{
    AddressMap findLibrary(const char *library)
    {
        char filename[0xFF] = {0},
             buffer[1024] = {0};
        FILE *fp = NULL;
        AddressMap addrMap;

        sprintf(filename, OBFUSCATE("/proc/self/maps"));

        fp = fopen(filename, OBFUSCATE("rt"));
        if (fp == NULL)
        {
            perror(OBFUSCATE("fopen"));
            goto done;
        }

        while (fgets(buffer, sizeof(buffer), fp))
        {
            if (strstr(buffer, library))
            {
                sscanf(buffer, "%lx-%lx", &addrMap.startAddress, &addrMap.endAddress);
                goto done;
            }
        }

    done:

        if (fp)
        {
            fclose(fp);
        }

        return addrMap;
    }

    ADDRESS getAbsoluteAddress(const char *libraryName, ADDRESS relativeAddr)
    {
        libBase = findLibrary(libraryName).startAddress;
        if (libBase == 0)
            return 0;
        return (reinterpret_cast<ADDRESS>(libBase + relativeAddr));
    }

    bool isLibraryLoaded(const char *libraryName)
    {
        // libLoaded = true;
        char line[512] = {0};
        FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
        if (fp != NULL)
        {
            while (fgets(line, sizeof(line), fp))
            {
                std::string a = line;
                if (strstr(line, libraryName))
                {
                    libLoaded = true;
                    return true;
                }
            }
            fclose(fp);
        }
        return false;
    }
    ADDRESS getRealOffset(ADDRESS address)
    {
        if (libBase == 0)
        {
            libBase = findLibrary(libName).startAddress;
        }
        return (libBase + address);
    }

    bool isUnityGame()
    {
        return isLibraryLoaded("libil2cpp.so");
    }

    uintptr_t string2Offset(const char *c)
    {
        int base = 16;
        // See if this function catches all possibilities.
        // If it doesn't, the function would have to be amended
        // whenever you add a combination of architecture and
        // compiler that is not yet addressed.
        static_assert(sizeof(uintptr_t) == sizeof(unsigned long) || sizeof(uintptr_t) == sizeof(unsigned long long),
                      "Please add string to handle conversion for this architecture.");

        // Now choose the correct function ...
        if (sizeof(uintptr_t) == sizeof(unsigned long))
        {
            return strtoul(c, nullptr, base);
        }

        // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
        return strtoull(c, nullptr, base);
    }

    char *readFileContent(const char *filename)
    {
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("Error opening file");
            return NULL;
        }

        // Tentukan ukuran file
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // Alokasikan buffer untuk menyimpan isi file
        char *buffer = (char *)malloc(file_size + 1);
        if (buffer == NULL)
        {
            perror("Error allocating memory");
            fclose(file);
            return NULL;
        }

        // Baca isi file ke dalam buffer
        size_t result = fread(buffer, 1, file_size, file);
        if (result != file_size)
        {
            perror("Error reading file");
            free(buffer);
            fclose(file);
            return NULL;
        }

        // Tambahkan null-terminator pada akhir buffer
        buffer[file_size] = '\0';

        fclose(file);
        return buffer;
    }
}

#endif
