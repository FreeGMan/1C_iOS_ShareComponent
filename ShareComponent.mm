//
//  ShareComponent.m
//  NativeAPI
//
//  Created by freegman on 13/08/2019.
//

//#include "./include/AddInDefBase.h"
#include <wchar.h>
#include <string.h>

#import "ShareComponent.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace COM_1C_SHARE_COMPONENT
{
    
};

using namespace COM_1C_SHARE_COMPONENT;

@interface DelegateClass : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@end

@implementation DelegateClass

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    
    //You can retrieve the actual UIImage
    UIImage *image = [info valueForKey:UIImagePickerControllerOriginalImage];
    //Or you can get the image url from AssetsLibrary
    NSURL *path = [info valueForKey:UIImagePickerControllerReferenceURL];
    NSString *filePath = [[NSString stringWithFormat:@"Documents/tmp_img."] stringByAppendingString:[path pathExtension]];
    NSString *fullTmpPath = [NSHomeDirectory() stringByAppendingPathComponent:filePath];
    
    NSLog(@"Selected assets:%@", path);
    NSLog(@"Path to image file:%@", fullTmpPath);
    
    [UIImagePNGRepresentation(image) writeToFile:fullTmpPath atomically:YES];
    [picker dismissViewControllerAnimated:YES completion:nil];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    
    NSString *fullTmpPath = [NSHomeDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"Documents/tmp_img.tmp"]];
    NSLog(@"Path to image file:%@", fullTmpPath);
    
    [[NSFileManager defaultManager] createFileAtPath:fullTmpPath
                                            contents:[[NSData alloc] init]
                                          attributes:nil];
    [picker dismissViewControllerAnimated:YES completion:nil];
}

@end

#if !defined(BUILD_DYNAMIC_LIBRARY)

namespace COM_1C_SHARE_COMPONENT
{
    
#endif //!BUILD_DYNAMIC_LIBRARY
    
//CShareComponent::CShareComponent() : piCnt(NULL)
//{
//
//}
//
//CShareComponent::~CShareComponent()
//{
//
//}
//
//void CShareComponent::setIConnect(IAddInDefBaseEx* piConnect)
//{
//    piCnt = piConnect;
//}
    
void CShareComponent::logMessage(std::wstring logText)
{
    NSString* convlogText = [[NSString alloc] initWithBytes:logText.data()
                                                     length:logText.size() * sizeof(wchar_t)
                                                   encoding:NSUTF32LittleEndianStringEncoding];
    NSLog(@"%@", convlogText);
}

wchar_t* CShareComponent::getGroupPath()
{
    NSString *groupPath = @"";
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *groupURL = [fileManager containerURLForSecurityApplicationGroupIdentifier:@"group.pb1cdev.a1realty"];
    if (groupURL) {
        groupPath = [groupURL absoluteString];
    }
    NSLog(@"%@", groupPath);
    
    const char* temp = [groupPath cStringUsingEncoding:NSUTF8StringEncoding];
    const size_t cSize = strlen(temp)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, temp, cSize);
    
    return wc;
}
    
void CShareComponent::shareFileByPath(std::wstring pathToFile, std::wstring descriptionFile)
{
    NSLog(@"Start prepare file");
    NSMutableArray* fileToShare = [[NSMutableArray alloc]init];
    @try {
        // Convert path to NString
        NSString* convPathToFile = [[NSString alloc] initWithBytes:pathToFile.data()
                                                             length:pathToFile.size() * sizeof(wchar_t)
                                                           encoding:NSUTF32LittleEndianStringEncoding];
        
        //Convert description to NString
        NSString* textToShare = [[NSString alloc] initWithBytes:descriptionFile.data()
                                                         length:descriptionFile.size() * sizeof(wchar_t)
                                                       encoding:NSUTF32LittleEndianStringEncoding];
        
        //Path may be array of path's. Convert to array
        NSArray* pathsArray = [convPathToFile componentsSeparatedByString:@"|"];
        
        //Get full path to file on Documents
        NSArray* pathsDocDirectory = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
        NSString* docDirectory = [pathsDocDirectory objectAtIndex:0];
        
        //Add existing file in array
        for (NSString* itemPath in pathsArray)
        {
            NSURL *fileURL = [NSURL fileURLWithPath:[docDirectory stringByAppendingPathComponent:itemPath]];
            [fileToShare addObject:fileURL];
            NSLog(@"%@", fileURL);
//            if ([[itemPath pathExtension] isEqualToString:@"png"] or [[itemPath pathExtension] isEqualToString:@"jpg"] or [[itemPath pathExtension] isEqualToString:@"jpeg"]
//                or [[itemPath pathExtension] isEqualToString:@"bmp"])
//            {
//                UIImage* imgForShare = [UIImage imageWithContentsOfFile:[docDirectory stringByAppendingPathComponent:itemPath]];
//                if (!imgForShare)
//                    continue;
//
//                [fileToShare addObject:imgForShare];
//
//            }
//            else
//            {
//                NSData* docForShare = [NSData dataWithContentsOfFile:[docDirectory stringByAppendingPathComponent:itemPath]];
//                if (!docForShare)
//                    continue;
//
//                [fileToShare addObject:docForShare];
//            }
            
        }
        
        //If no existing file come back
        if (fileToShare.count == 0)
            return;
        
        //Add description
        [fileToShare addObject:textToShare];
        
            // Your statements here
    }
    @catch (NSException * e) {
        NSLog(@"Exception: %@", e);
        return;
    }
    @finally {
        NSLog(@"Complite prepare file");
    }
    //Open assync
    dispatch_async(dispatch_get_main_queue(), ^(void){
        //Create self View Controller	
        UIViewController* controllerViewNew = [UIApplication sharedApplication].keyWindow.rootViewController;
        
        UIActivityViewController* activityVC = [[UIActivityViewController alloc] initWithActivityItems:[fileToShare copy] applicationActivities:nil];
        
        if (activityVC == nil)
            return;
        
        NSArray* excludeActivities = @[UIActivityTypeAirDrop,
                                       UIActivityTypePrint,
                                       UIActivityTypeAssignToContact,
                                       UIActivityTypeSaveToCameraRoll,
                                       UIActivityTypeAddToReadingList,
                                       UIActivityTypePostToFlickr,
                                       UIActivityTypePostToVimeo];
        
        activityVC.excludedActivityTypes = excludeActivities;
        
        //Open View
        //for iPhone
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            
            [controllerViewNew presentViewController:activityVC animated:YES completion:nil];
            
        }
        //for iPad
        else {
            // Change Rect as required
            UIPopoverController* popup = [[UIPopoverController alloc] initWithContentViewController:activityVC];
            [popup presentPopoverFromRect:CGRectMake(controllerViewNew.view.frame.size.width/2, controllerViewNew.view.frame.size.height/4, 0, 0)inView:controllerViewNew.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        }
    });
}

void CShareComponent::addObjecInListFile(std::wstring objCodeInc, std::wstring objNameInc)
{
    @try {
        NSLog(@"Start add object to list!");
        //Conver incoming parametrs to NSString format
        NSString* objCode = [[NSString alloc] initWithBytes:objCodeInc.data()
                                                     length:objCodeInc.size() * sizeof(wchar_t)
                                                   encoding:NSUTF32LittleEndianStringEncoding];
        NSString* objName = [[NSString alloc] initWithBytes:objNameInc.data()
                                                     length:objNameInc.size() * sizeof(wchar_t)
                                                   encoding:NSUTF32LittleEndianStringEncoding];
        if (!objCode or !objName) {
            NSLog(@"Parametr's is Empty!");
            return;
        }
        NSLog(@"Add obgect %@ with code %@", objName, objCode);
        NSDictionary *newObj = @{
            @"objCode":objCode,
            @"objName":objName
        };
    
        //Add new Object in new list
        NSMutableArray *objectListNew = [[NSMutableArray alloc] init];
        [objectListNew addObject:newObj];
        
        //Check for existing file list and extended
        //Path to group app directory and file
        NSFileManager *fileMan = [NSFileManager defaultManager];
        NSURL *groupURL = [fileMan containerURLForSecurityApplicationGroupIdentifier:@"group.pb1cdev.a1realty"];
        NSURL *groupCachePath = [groupURL URLByAppendingPathComponent:@"Library/Caches"];
        NSURL *pathFileLastObject = [groupCachePath URLByAppendingPathComponent:@"lastOobjects.json"];
        
        NSError *err = nil;
        if ([fileMan fileExistsAtPath:[pathFileLastObject path]]){
            //Add old Object after new
            NSData *dataJSON = [NSData dataWithContentsOfURL:pathFileLastObject];
            NSArray *objectList = [[NSArray alloc] init];
            if (dataJSON) {
                NSError *err = nil;
                objectList = [NSJSONSerialization JSONObjectWithData:dataJSON options:kNilOptions error:&err];
                if (objectList) {
                    for (NSDictionary* objOld in objectList ) {
                        if (![objCode isEqualToString:objOld[@"objCode"]]){
                            [objectListNew addObject:objOld];
                        }
                        if (objectListNew.count > 20) break;
                    }
                }else{
                    NSLog(@"Exception on the DeSerialization: %@", [err localizedDescription]);
                }
            }
        }else{
            NSLog(@"Old file not reacheble: %@", [err localizedDescription]);
        }
    
        //Write new object list in file
        if (objectListNew.count > 0) {
            NSError *err = nil;
            NSData *dataJSONNew = [NSJSONSerialization dataWithJSONObject:objectListNew options:0 error:&err];
            if (err) {
                NSLog(@"%@", [err localizedDescription]);
                return;
            }
            if ([dataJSONNew writeToURL:pathFileLastObject options:NSDataWritingAtomic error:&err] == YES){
                NSLog(@"File will be write successful");
            }else{
                NSLog(@"Write returned error: %@", [err localizedDescription]);
            };
        }
    }
    @catch (NSException *err) {
        NSLog(@"Exception: %@", err.reason);
        return;
    }
    @finally {
        NSLog(@"Complite prepare file");
    }
}

void CShareComponent::pickPhotoFromLibrary()
{
    NSLog(@"Start picking photo");
    
    //Delete old folder with image
    NSFileManager *fileMan = [NSFileManager defaultManager];
    NSString *docDirectory = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSArray *contents = [fileMan contentsOfDirectoryAtPath:docDirectory error:nil];
    NSString *item;
    for (item in contents)
    {
//        NSLog(@"Photo for delete - %@", item);
        if ([[[item lastPathComponent] stringByDeletingPathExtension] isEqualToString:@"tmp_img"])
        {
            [fileMan removeItemAtPath:[docDirectory stringByAppendingPathComponent:item] error:nil];
        }
    }
    
    //Open assync
    dispatch_async(dispatch_get_main_queue(), ^(void){
        //Create self View Controller
        UIViewController* controllerViewNew = [UIApplication sharedApplication].keyWindow.rootViewController;
        
        //Create delegate object
        DelegateClass *pickerDelegate = [[DelegateClass alloc] init];
        
        UIImagePickerController *pickerLibrary = [[UIImagePickerController alloc] init];
        pickerLibrary.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        pickerLibrary.delegate = pickerDelegate;
        [controllerViewNew presentModalViewController:pickerLibrary animated:YES];

    });

}
    
#if !defined(BUILD_DYNAMIC_LIBRARY)
    
}

#endif //!BUILD_DYNAMIC_LIBRARY
