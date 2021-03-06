//
//  MTSoupView.h
//  MacTierra
//
//  Created by Simon Fraser on 8/16/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "MTCompositedGLView.h"

namespace MacTierra {
    class World;
};

@class MTWorldController;

@interface MTSoupView : MTCompositedGLView
{
    IBOutlet MTWorldController*     mWorldController;
    IBOutlet NSArrayController*     mGenotypesArrayController;
    
    MacTierra::World*   mWorld;
    
    int                 mSoupWidth;
    int                 mSoupHeight;
    
    BOOL                zoomToFit;
    BOOL                showCells;
    BOOL                showInstructionPointers;
    BOOL                showFecundity;
    
    NSString*           focusedCreatureName;

}

- (void)setWorld:(MacTierra::World*)inWorld;
- (MacTierra::World*)world;

@property (assign) BOOL zoomToFit;
@property (assign) BOOL showCells;
@property (assign) BOOL showInstructionPointers;
@property (assign) BOOL showFecundity;

@property (assign) NSString* focusedCreatureName;


@end
