/*
 * Process Hacker Window Explorer - 
 *   main program
 * 
 * Copyright (C) 2010 wj32
 * 
 * This file is part of Process Hacker.
 * 
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "wndexp.h"
#include "resource.h"

VOID NTAPI LoadCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI UnloadCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI ShowOptionsCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI MenuItemCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI MainWindowShowingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI ProcessPropertiesInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI ProcessMenuInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

VOID NTAPI ThreadMenuInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    );

PPH_PLUGIN PluginInstance;
PH_CALLBACK_REGISTRATION PluginLoadCallbackRegistration;
PH_CALLBACK_REGISTRATION PluginUnloadCallbackRegistration;
PH_CALLBACK_REGISTRATION PluginShowOptionsCallbackRegistration;
PH_CALLBACK_REGISTRATION PluginMenuItemCallbackRegistration;
PH_CALLBACK_REGISTRATION MainWindowShowingCallbackRegistration;
PH_CALLBACK_REGISTRATION ProcessPropertiesInitializingCallbackRegistration;
PH_CALLBACK_REGISTRATION ProcessMenuInitializingCallbackRegistration;
PH_CALLBACK_REGISTRATION ThreadMenuInitializingCallbackRegistration;

LOGICAL DllMain(
    __in HINSTANCE Instance,
    __in ULONG Reason,
    __reserved PVOID Reserved
    )
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        {
            PH_PLUGIN_INFORMATION info;

            info.DisplayName = L"Window Explorer";
            info.Author = L"wj32";
            info.Description = L"View and manipulate windows.";
            info.HasOptions = FALSE;

            PluginInstance = PhRegisterPlugin(L"ProcessHacker.WindowExplorer", Instance, &info);

            if (!PluginInstance)
                return FALSE;

            PhRegisterCallback(
                PhGetPluginCallback(PluginInstance, PluginCallbackLoad),
                LoadCallback,
                NULL,
                &PluginLoadCallbackRegistration
                );
            //PhRegisterCallback(
            //    PhGetPluginCallback(PluginInstance, PluginCallbackUnload),
            //    UnloadCallback,
            //    NULL,
            //    &PluginUnloadCallbackRegistration
            //    );
            //PhRegisterCallback(
            //    PhGetPluginCallback(PluginInstance, PluginCallbackShowOptions),
            //    ShowOptionsCallback,
            //    NULL,
            //    &PluginShowOptionsCallbackRegistration
            //    );
            PhRegisterCallback(
                PhGetPluginCallback(PluginInstance, PluginCallbackMenuItem),
                MenuItemCallback,
                NULL,
                &PluginMenuItemCallbackRegistration
                );

            PhRegisterCallback(
                PhGetGeneralCallback(GeneralCallbackMainWindowShowing),
                MainWindowShowingCallback,
                NULL,
                &MainWindowShowingCallbackRegistration
                );
            PhRegisterCallback(
                PhGetGeneralCallback(GeneralCallbackProcessPropertiesInitializing),
                ProcessPropertiesInitializingCallback,
                NULL,
                &ProcessPropertiesInitializingCallbackRegistration
                );
            PhRegisterCallback(
                PhGetGeneralCallback(GeneralCallbackProcessMenuInitializing),
                ProcessMenuInitializingCallback,
                NULL,
                &ProcessMenuInitializingCallbackRegistration
                );
            PhRegisterCallback(
                PhGetGeneralCallback(GeneralCallbackThreadMenuInitializing),
                ThreadMenuInitializingCallback,
                NULL,
                &ThreadMenuInitializingCallbackRegistration
                );

            //{
            //    static PH_SETTING_CREATE settings[] =
            //    {
            //        { IntegerSettingType, SETTING_NAME_ENABLE_ETW_MONITOR, L"1" },
            //        { IntegerSettingType, SETTING_NAME_ETWSYS_ALWAYS_ON_TOP, L"0" },
            //        { IntegerPairSettingType, SETTING_NAME_ETWSYS_WINDOW_POSITION, L"400,400" },
            //        { IntegerPairSettingType, SETTING_NAME_ETWSYS_WINDOW_SIZE, L"500,400" },
            //        { IntegerPairSettingType, SETTING_NAME_MEMORY_LISTS_WINDOW_POSITION, L"400,400" }
            //    };

            //    PhAddSettings(settings, sizeof(settings) / sizeof(PH_SETTING_CREATE));
            //}
        }
        break;
    }

    return TRUE;
}

VOID NTAPI LoadCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    NOTHING;
}

VOID NTAPI UnloadCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    NOTHING;
}

VOID NTAPI ShowOptionsCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    NOTHING;
}

VOID NTAPI MenuItemCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    PPH_PLUGIN_MENU_ITEM menuItem = Parameter;

    switch (menuItem->Id)
    {
    case ID_VIEW_WINDOWS:
        {
            WE_WINDOW_SELECTOR selector;

            selector.Type = WeWindowSelectorAll;
            WeCreateWindowsDialog(PhMainWndHandle, &selector);
        }
        break;
    }
}

VOID NTAPI MainWindowShowingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    PhPluginAddMenuItem(PluginInstance, PH_MENU_ITEM_LOCATION_VIEW, L"System Information", ID_VIEW_WINDOWS, L"Windows", NULL);
}

VOID NTAPI ProcessPropertiesInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    NOTHING;
}

VOID NTAPI ProcessMenuInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    PPH_PLUGIN_MENU_INFORMATION menuInfo = Parameter;
    PPH_PROCESS_ITEM processItem;
    PPH_EMENU_ITEM miscMenu;

    if (menuInfo->u.Process.NumberOfProcesses == 1)
        processItem = menuInfo->u.Process.Processes[0];
    else
        processItem = NULL;

    miscMenu = PhFindEMenuItem(menuInfo->Menu, 0, L"Miscellaneous", 0);

    if (miscMenu)
    {
        //PhInsertEMenuItem(miscMenu, PhPluginCreateEMenuItem(PluginInstance, 0, ID_PROCESS_UNLOADEDMODULES, L"Unloaded Modules", processItem), -1);
    }
}

VOID NTAPI ThreadMenuInitializingCallback(
    __in_opt PVOID Parameter,
    __in_opt PVOID Context
    )
{
    PPH_PLUGIN_MENU_INFORMATION menuInfo = Parameter;
    PPH_THREAD_ITEM threadItem;
    ULONG insertIndex;
    PPH_EMENU_ITEM menuItem;

    if (menuInfo->u.Thread.NumberOfThreads == 1)
        threadItem = menuInfo->u.Thread.Threads[0];
    else
        threadItem = NULL;

    if (menuItem = PhFindEMenuItem(menuInfo->Menu, 0, L"Resume", 0))
        insertIndex = PhIndexOfEMenuItem(menuInfo->Menu, menuItem) + 1;
    else
        insertIndex = 0;

    //PhInsertEMenuItem(menuInfo->Menu, menuItem = PhPluginCreateEMenuItem(PluginInstance, 0, ID_THREAD_CANCELIO,
    //    L"Cancel I/O", threadItem), insertIndex);

    if (!threadItem) menuItem->Flags |= PH_EMENU_DISABLED;
}
