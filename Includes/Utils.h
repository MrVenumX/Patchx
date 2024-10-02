#ifndef UTILS
#define UTILS

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
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

typedef __SIZE_TYPE__ size_t;

struct AddressMap
{
    ADDRESS startAddress;
    ADDRESS endAddress;
};

namespace Utils
{
    AddressMap findLibrary(const char *library)
    {
        char filename[0xFF] = {0}, buffer[1024] = {0};
        FILE *fp = nullptr;
        AddressMap addrMap;

        sprintf(filename, "/proc/self/maps");

        fp = fopen(filename, "rt");
        if (fp == nullptr)
        {
            perror("fopen");
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
        FILE *fp = fopen("/proc/self/maps", "rt");
        if (fp != nullptr)
        {
            while (fgets(line, sizeof(line), fp))
            {
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

    ADDRESS getLibOffset(ADDRESS address)
    {
        if (libBase == 0)
        {
            libBase = findLibrary(libName).startAddress;
        }
        return (libBase - address);
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
        if (file == nullptr)
        {
            perror("Error opening file");
            return nullptr;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *buffer = (char *)malloc(file_size + 1);
        if (buffer == nullptr)
        {
            perror("Error allocating memory");
            fclose(file);
            return nullptr;
        }

        size_t result = fread(buffer, 1, file_size, file);
        if (result != file_size)
        {
            perror("Error reading file");
            free(buffer);
            fclose(file);
            return nullptr;
        }

        buffer[file_size] = '\0';

        fclose(file);
        return buffer;
    }
}

#endif
