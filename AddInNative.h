
#ifndef __ADDINNATIVE_H__
#define __ADDINNATIVE_H__

#include "./include/ComponentBase.h"
#include "./include/AddInDefBase.h"
#include "./include/IMemoryManager.h"
#include "ShareComponent.h"

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace COM_1C_SHARE_COMPONENT
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

static const wchar_t g_kClassNames[] = L"ShareComponent";
static const wchar_t g_ClassShareComponent[] = L"ShareComponent";

static WcharWrapper s_classNameShareComponent(g_ClassShareComponent);
static WcharWrapper s_kClassNames(g_kClassNames);

///////////////////////////////////////////////////////////////////////////////
class ShareComponent : public IComponentBase
{
public:
    enum Props
    {
        ePropGroupPath = 0,
        ePropImagePick,
        ePropLast      // Always last
    };

    enum Methods
    {
        eMethShareFile = 0,
        eMethAddObjToList,
        eMethPickPhoto,
        eMethLast      // Always last
    };

    ShareComponent(void);
    virtual ~ShareComponent();
    // IInitDoneBase
    virtual bool ADDIN_API Init(void*);
    virtual bool ADDIN_API setMemManager(void* mem);
    virtual long ADDIN_API GetInfo();
    virtual void ADDIN_API Done();
    // ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T**);
    virtual long ADDIN_API GetNProps();
    virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
    virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);
    virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);
    virtual bool ADDIN_API IsPropReadable(const long lPropNum);
    virtual bool ADDIN_API IsPropWritable(const long lPropNum);
    virtual long ADDIN_API GetNMethods();
    virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, 
                            const long lMethodAlias);
    virtual long ADDIN_API GetNParams(const long lMethodNum);
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum,
                            tVariant *pvarParamDefValue);   
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);
    virtual bool ADDIN_API CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray);
    virtual bool ADDIN_API CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);
    // ILocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);

private:
    long findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const;
    
    // For Internal usage / Для внутреннего использования
    const wchar_t* groupPath;
    
    IAddInDefBaseEx    *m_iConnect;
    IMemoryManager     *m_iMemory;
    CShareComponent    shareComponent;
};

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

};

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

#endif //__ADDINNATIVE_H__
