/**
 * File    : Filesystem.cpp
 * Author  : Emir Uner
 * Summary : File and directory functions for linux.
 */

/**
 * This file is part of XCOM.
 *
 * Copyright (C) 2003 Emir Uner
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <xcom/Types.hpp>

#include <sys/types.h>
//#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <direct.h>

#include <sstream>

#include "Shlwapi.h"

using namespace xcom;

extern "C"
Int xcomFileExists(Char const* file)
{
	return PathFileExists(file);
}

namespace
{

// Get executable's name (as specified by the user) using /proc
Char* executablePath()
{
	TCHAR szFileName[MAX_PATH + 1];
	GetModuleFileName(NULL, szFileName, MAX_PATH + 1);
	return xcomDuplicateString(szFileName);
}

} // namespace <unnamed>

extern "C"
Char* xcomMakePathAbsolute(Char const* relative)
{
    if(relative[0] == '/')
    {
        return xcomDuplicateString(relative);
    }

    char buf[MAX_PATH];
    
    if(getcwd(buf, MAX_PATH) == 0)
    {
        return 0;
    }
    
    size_t len = strlen(buf);
    
    buf[len] = '/';
    buf[len + 1] = '\0';
    
    strcat(buf, relative);
    
    return xcomDuplicateString(buf);
}

extern "C"
Char* xcomMakeLibraryFilename(Char const* libraryName)
{
    char buf[MAX_PATH];
    
    strcpy(buf, libraryName);
    strcat(buf, ".so");
    
    return xcomDuplicateString(buf);
}

extern "C"
Char* xcomMakeLibraryName(xcom::Char const* pathName)
{
    char buf[MAX_PATH];
    char const* tmp = pathName;
    char* target = buf;
    
    while(*tmp++ != '\0'); // Go to the end
    while(tmp != pathName) // Either go to beginning or one past of a '/'
    {
        if(*tmp == '/')
        {
            ++tmp;
            break;
        }
        
        --tmp;
    }
    
    while(*tmp != '.')
    {
        *target = *tmp;
        ++target;
        ++tmp;
    }

    *target = '\0';
    
    return xcomDuplicateString(buf);
}

extern "C"
Char* xcomConfigFileForApplication()
{
    char buf[MAX_PATH];
    String path(String::adopt(executablePath()));
    
    strcpy(buf, path.c_str());
    strcat(buf, ".config");

    return xcomDuplicateString(buf);
}

extern "C"
Char* xcomCombinePathWithFile(Char const* path, Char const* filename)
{
    char buf[MAX_PATH];

    strcpy(buf, path);
    
    while(*path++ != '\0');
    --path;
    --path;

    if(*path != '/')
    {
        char slash[2] = { '/', '\0' };    
        strcat(buf, slash);
    }

    strcat(buf, filename);

    return xcomDuplicateString(buf);
}

    
