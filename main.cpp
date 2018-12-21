#include <windows.h>

unsigned int base;

struct GZArgs {
    unsigned int world_ptr;
    unsigned int x;
    unsigned int y;
} ;
void GenerateZoneThread(GZArgs* args){
    typedef void(__thiscall* cube_World_GenerateZone_t)(unsigned int, unsigned int, unsigned int);
    auto cube_World_GenerateZone = (cube_World_GenerateZone_t)(base + 0x1E4850);
    cube_World_GenerateZone(args->world_ptr, args->x, args->y);
    delete args;
}

void __thiscall GenerateZone(unsigned int world_ptr, unsigned int x, unsigned int y){

    //edges
    GZArgs* args1 = new GZArgs {world_ptr, x, y};
    HANDLE h1 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args1,0,0);

    GZArgs* args2 = new GZArgs {world_ptr, x+1, y};
    HANDLE h2 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args2,0,0);

    GZArgs* args3 = new GZArgs {world_ptr, x-1, y};
    HANDLE h3 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args3,0,0);

    GZArgs* args4 = new GZArgs {world_ptr, x, y+1};
    HANDLE h4 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args4,0,0);

    GZArgs* args5 = new GZArgs {world_ptr, x, y-1};
    HANDLE h5 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args5,0,0);


    //corners
    GZArgs* args6 = new GZArgs {world_ptr, x+1, y-1};
    HANDLE h6 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args6,0,0);

    GZArgs* args7 = new GZArgs {world_ptr, x+1, y+1};
    HANDLE h7 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args7,0,0);

    GZArgs* args8 = new GZArgs {world_ptr, x-1, y-1};
    HANDLE h8 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args8,0,0);

    GZArgs* args9 = new GZArgs {world_ptr, x-1, y+1};
    HANDLE h9 = CreateThread(0,0,(LPTHREAD_START_ROUTINE)GenerateZoneThread,args9,0,0);

    WaitForSingleObject(h1,INFINITE);
    WaitForSingleObject(h2,INFINITE);
    WaitForSingleObject(h3,INFINITE);
    WaitForSingleObject(h4,INFINITE);
    WaitForSingleObject(h5,INFINITE);
    WaitForSingleObject(h6,INFINITE);
    WaitForSingleObject(h7,INFINITE);
    WaitForSingleObject(h8,INFINITE);
    WaitForSingleObject(h9,INFINITE);
}

void WriteJMP(BYTE* location, BYTE* newFunction){
    DWORD dwOldProtection;
    VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    location[0] = 0xE9; //jmp
    *((DWORD*)(location + 1)) = (DWORD)(( (unsigned INT32)newFunction - (unsigned INT32)location ) - 5);
    VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}

void WriteCall(BYTE* location, BYTE* newFunction){
    DWORD dwOldProtection;
    VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    location[0] = 0xE8; //call
    *((DWORD*)(location + 1)) = (DWORD)(( (unsigned INT32)newFunction - (unsigned INT32)location ) - 5);
    VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}

extern "C" __declspec(dllexport) bool APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            base = (unsigned int)GetModuleHandle(NULL);

            WriteCall((BYTE*)base+0x6ACD1, (BYTE*)GenerateZone);

            break;

    }
    return true;
}
