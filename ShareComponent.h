//
//  ShareComponent.h
//  NativeAPI
//
//  Created by freegman on 13/08/2019.
//

#include "ConversionWchar.h"
#include <string>
#include "./include/mobile.h"
//#import <Foundation/Foundation.h>
//#import <UIKit/UIKit.h>

#ifndef __NATIVEAPISHARECOMPONENT_H__
#define __NATIVEAPISHARECOMPONENT_H__

#if defined(__OBJC__)

@class ShareComponent;

#endif //__OBJC__

//class IAddInDefBaseEx;
//
//class IMemoryManager;

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace COM_1C_SHARE_COMPONENT
{
    
#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

class CShareComponent
{
    
public:
//    CShareComponent();
//    ~CShareComponent();
//    
//    void setIConnect(IAddInDefBaseEx* piConnect);
    void logMessage(std::wstring logText);
    wchar_t* getGroupPath();
    void shareFileByPath(std::wstring pathToFile, std::wstring descriptionFile);
    void addObjecInListFile(std::wstring objCodeInc, std::wstring objNameInc);
    void pickPhotoFromLibrary();
//private:
//    IAddInDefBaseEx*    piCnt;
    
};
    
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)
    
};

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

#endif //__NATIVEAPISHARECOMPONENT_H__


