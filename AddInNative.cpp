
#include "stdafx.h"
#include "AddInNative.h"
#include "ConversionWchar.h"
#include <stdio.h>

#if defined(__ANDROID__) || defined(__APPLE__)

#include <wchar.h>

#endif //__ANDROID__ || __APPLE__

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace COM_1C_SHARE_COMPONENT
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY
    
static const wchar_t *g_PropNames[] =
   {
       L"GroupPath",
       L"ImagePick"
       
   };

static const wchar_t *g_MethodNames[] =
   {
        L"ShareFile",
        L"AddObjToList",
        L"PickPhoto"
   };

static const wchar_t *g_PropNamesRu[] =
   {
       L"ПутьКФайламГруппы",
       L"ФотоВыбраны"
       
   };

static const wchar_t *g_MethodNamesRu[] =
    {
        L"ПоделитьсяФайлом",
        L"ДобавитьОбъектВФайл",
        L"ВыбратьФото"
        
    };

static const wchar_t g_ComponentNameType[] = L"com_1c_ShareComponentExtension";
    
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

static const char g_ComponentName[] = "com_1c_ShareComponent";

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

// This component supports 2.1 version
const long g_VersionAddIn = 2100;

// For Internal usage / Для внутреннего использования
bool imagePick;

static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface = new ShareComponent;
        return (long)*pInterface;
    }
    return 0;
}
    
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}
    
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pInterface)
{
    if(!*pInterface)
        return -1;

    delete *pInterface;
    *pInterface = 0;
    return 0;
}
    
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    return s_kClassNames;
}
    
//---------------------------------------------------------------------------//
ShareComponent::ShareComponent() : m_iConnect(0), m_iMemory(0)
{
}
    
//---------------------------------------------------------------------------//
ShareComponent::~ShareComponent()
{
}
    
/////////////////////////////////////////////////////////////////////////////
// IInitDoneBase
//---------------------------------------------------------------------------//
bool ShareComponent::Init(void* pConnection)
{
    shareComponent.logMessage(L"Init Start");
    m_iConnect = (IAddInDefBaseEx*)pConnection;
    if (m_iConnect)
    {
        imagePick = false;
        groupPath = shareComponent.getGroupPath();
        shareComponent.logMessage(groupPath);
//        shareComponent.setIConnect(m_iConnect);
        shareComponent.logMessage(L"Init Complite");
        return true;
    }
    shareComponent.logMessage(L"Init Fail");
    return false;
}

//---------------------------------------------------------------------------//
bool ShareComponent::setMemManager(void* mem)
{
    shareComponent.logMessage(L"Set Memory Start");
    m_iMemory = (IMemoryManager*)mem;
    if (m_iMemory != 0)
        shareComponent.logMessage(L"Set Memory Complite");
    else
        shareComponent.logMessage(L"ISet Memory Fail");
    
    return m_iMemory != 0;
}
    
//---------------------------------------------------------------------------//
long ShareComponent::GetInfo()
{ 
    // Component should put supported component technology version
    return g_VersionAddIn;
}
    
//---------------------------------------------------------------------------//
void ShareComponent::Done()
{
    m_iConnect = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool ShareComponent::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{
    shareComponent.logMessage(L"Register Start");
    const wchar_t *wsExtension = g_ComponentNameType;
    uint32_t iActualSize = static_cast<uint32_t>(::wcslen(wsExtension) + 1);
    shareComponent.logMessage(wsExtension);
    
    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
        {
            convToShortWchar(wsExtensionName, wsExtension, iActualSize);
            shareComponent.logMessage(L"Register Complite");
            return true;
        }
        shareComponent.logMessage(L"Register Fail allocate memory");
    }
    
    shareComponent.logMessage(L"Register Fail m_iMemory");
    return false; 
}
    
//---------------------------------------------------------------------------//
long ShareComponent::GetNProps()
{ 
    // You may delete next lines and add your own implementation code here
    return ePropLast;
}
    
//---------------------------------------------------------------------------//
long ShareComponent::FindProp(const WCHAR_T* wsPropName)
{ 
    long plPropNum = -1;
    wchar_t* propName = 0;
    convFromShortWchar(&propName, wsPropName);
    
    plPropNum = findName(g_PropNames, propName, ePropLast);

    if (plPropNum == -1)
        plPropNum = findName(g_PropNamesRu, propName, ePropLast);

    delete[] propName;

    return plPropNum;
}
    
//---------------------------------------------------------------------------//
const WCHAR_T* ShareComponent::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= ePropLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsPropName = NULL;

    switch(lPropAlias)
    {
    case 0: // First language (english)
        wsCurrentName = (wchar_t*)g_PropNames[lPropNum];
        break;
    case 1: // Second language (local)
        wsCurrentName = (wchar_t*)g_PropNamesRu[lPropNum];
        break;
    default:
        return 0;
    }
    
    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);

    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
    }

    return wsPropName;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    switch(lPropNum)
    {
        case ePropGroupPath:
        {
            uint32_t iActualSize = static_cast<uint32_t>(wcslen(groupPath) + 1);
            
            if(m_iMemory->AllocMemory((void**)&pvarPropVal->pwstrVal, iActualSize * sizeof(WCHAR_T))){
                convToShortWchar(&pvarPropVal->pwstrVal, groupPath, iActualSize);
                pvarPropVal->wstrLen = iActualSize - 1;
                TV_VT(pvarPropVal) = VTYPE_PWSTR;
            }
            break;
        }
        case ePropImagePick:
        {
            if(m_iMemory->AllocMemory((void**)&pvarPropVal->bVal, sizeof(BOOL))){
                pvarPropVal->bVal = imagePick;
                TV_VT(pvarPropVal) = VTYPE_BOOL;
                if (imagePick) {
                    shareComponent.logMessage(L"imagePick = true");
                }
                else{
                    shareComponent.logMessage(L"imagePick = false");
                };
                
            }
            break;
        }
        default:
            return false;
    }
    return true;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::SetPropVal(const long lPropNum, tVariant *varPropVal)
{ 
    return true;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::IsPropReadable(const long lPropNum)
{ 
    switch(lPropNum)
    {
        case ePropGroupPath:
        case ePropImagePick:
            return true;
        default:
            return false;
    }
    return false;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::IsPropWritable(const long lPropNum)
{
    return false;
}

//---------------------------------------------------------------------------//
long ShareComponent::GetNMethods()
{ 
    return eMethLast;
}
    
//---------------------------------------------------------------------------//
long ShareComponent::FindMethod(const WCHAR_T* wsMethodName)
{ 
    long plMethodNum = -1;
    wchar_t* name = 0;
    convFromShortWchar(&name, wsMethodName);

    plMethodNum = findName(g_MethodNames, name, eMethLast);

    if (plMethodNum == -1)
        plMethodNum = findName(g_MethodNamesRu, name, eMethLast);

    delete[] name;

    return plMethodNum;
}
    
//---------------------------------------------------------------------------//
const WCHAR_T* ShareComponent::GetMethodName(const long lMethodNum, const long lMethodAlias)
{ 
    if (lMethodNum >= eMethLast)
        return NULL;

    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsMethodName = NULL;

    switch(lMethodAlias)
    {
    case 0: // First language (english)
        wsCurrentName = (wchar_t*)g_MethodNames[lMethodNum];
        break;
    case 1: // Second language (local)
        wsCurrentName = (wchar_t*)g_MethodNamesRu[lMethodNum];
        break;
    default: 
        return 0;
    }

    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);

    if (m_iMemory && wsCurrentName)
    {
        if(m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
    }

    return wsMethodName;
}
    
//---------------------------------------------------------------------------//
long ShareComponent::GetNParams(const long lMethodNum)
{ 
    switch (lMethodNum)
    { 
        case eMethShareFile:
        case eMethAddObjToList:
            return 2;
    }
    return 0;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::GetParamDefValue(const long lMethodNum, const long lParamNum,
                        tVariant *pvarParamDefValue)
{ 
    // There are no parameter values by default
    TV_VT(pvarParamDefValue) = VTYPE_EMPTY;

    switch (lMethodNum)
    {
        case eMethShareFile:
        case eMethAddObjToList:
        {
            // Only first parameter
            if (lParamNum == 0)
                return true;
        }
    }
    return false;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::HasRetVal(const long lMethodNum)
{
    switch (lMethodNum)
    {
        default:
            return false;
    }
    return false;
}
    
//---------------------------------------------------------------------------//
bool ShareComponent::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{
    switch(lMethodNum)
    {
        case eMethShareFile:
        {
            // One parameter
            if ((lSizeArray != 2) || TV_VT(paParams) != VTYPE_PWSTR)
                return false;
            
            wchar_t* pathToFile = NULL;
            wchar_t* descriptionFile = NULL;
            convFromShortWchar(&pathToFile, (paParams)->pwstrVal);
            convFromShortWchar(&descriptionFile, (paParams+1)->pwstrVal);
            shareComponent.shareFileByPath(pathToFile, descriptionFile);
            break;
        }
        case eMethAddObjToList:
        {
            // One parameter
            if ((lSizeArray != 2) || TV_VT(paParams) != VTYPE_PWSTR)
                return false;
            
            wchar_t* objCodeInc = NULL;
            wchar_t* objNameInc = NULL;
            convFromShortWchar(&objCodeInc, (paParams)->pwstrVal);
            convFromShortWchar(&objNameInc, (paParams+1)->pwstrVal);
            shareComponent.addObjecInListFile(objCodeInc, objNameInc);
            break;
        }
        case eMethPickPhoto:
        {
            shareComponent.pickPhotoFromLibrary();
            break;
        }
    default:
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------//
bool ShareComponent::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{
    switch(lMethodNum)
    {
        default:
            return false;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
// ILocaleBase
//---------------------------------------------------------------------------//
void ShareComponent::SetLocale(const WCHAR_T* loc)
{
}

/////////////////////////////////////////////////////////////////////////////
// Other

//---------------------------------------------------------------------------//
long ShareComponent::findName(const wchar_t* names[], const wchar_t* name,
                        const uint32_t size) const
{
    long ret = -1;
    for (uint32_t i = 0; i < size; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

//---------------------------------------------------------------------------//
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

};

namespace COM_1C_SHARE_COMPONENT
{

    static LPCVOID addin_exports[] =
    {
        "GetClassObject", (LPCVOID)GetClassObject,
        "DestroyObject", (LPCVOID)DestroyObject,
        "GetClassNames", (LPCVOID)GetClassNames,
        "SetPlatformCapabilities", (LPCVOID)SetPlatformCapabilities,
        NULL
    };

    DECLARE_DLL((const char*)g_ComponentName, addin_exports);
}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY
