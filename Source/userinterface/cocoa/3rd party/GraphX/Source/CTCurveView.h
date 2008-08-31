//
//  CTCurveView.h
//
//  Created by Chad Weider on Fri May 14 2004.
//  Copyright (c) 2005 Cotingent. All rights reserved.
//


#import <Cocoa/Cocoa.h>
#import "CTGraphView.h"

@protocol CTCurveViewDataSource
- (double)yValueForXValue:(double)x;
@end

@protocol CTCurveViewDelegate
- (void)hasDrawnFirstSegmentDataPoint:(NSPoint)dataPoint atViewPoint:(NSPoint)viewPoint inRect:(NSRect)rect withOrigin:(NSPoint)viewOrigin;
- (void)hasDrawnSegmentDataPoint     :(NSPoint)dataPoint atViewPoint:(NSPoint)viewPoint inRect:(NSRect)rect withOrigin:(NSPoint)viewOrigin;
- (void)hasDrawnLastSegmentDataPoint :(NSPoint)dataPoint atViewPoint:(NSPoint)viewPoint inRect:(NSRect)rect withOrigin:(NSPoint)viewOrigin;
@end



@interface CTCurveView : CTGraphView
{
  IBOutlet id <CTCurveViewDataSource> dataSource;   //object that will give graph values for drawing the curve
  IBOutlet id <CTCurveViewDelegate  > delegate  ;   //object that will be notified when key events occur
  
  
  float res;  //Determines number of pixels per point on curve
              // Frequency of Point samples taken from DataSource to form the continous curve
              // *has a major effect on preformance - use high values if DataSource method is slow
  float xres; //Resolution used during graphing process - may be adjusted(lower) from res during live resize for preformance
  BOOL  approx; //flag for whether or not to allow approximations during live resizes
  
  
  BOOL drawGraphFlag , drawFillFlag;  //Flags to turn on/off different components of CTGraphView
  
  float curveLineWidth;   //width of the curve
  
  NSBezierPath *curve;
  NSBezierPath *displacement;
}

- (void)drawGraph:(NSRect)rect;   //Draws the Actual Graph - Curve and area under Curve (if Flags are Set)

//Customization Methods
- (void)setRes:(float)resolution;
- (void)setApproximateOnLiveResize:(BOOL)state;

- (void)setShowCurve :(BOOL)state;
- (void)setShowFill  :(BOOL)state;

- (void)setCurveColor:(NSColor *)color;
- (void)setFillColor :(NSColor *)color;


//State Methods
- (float)res;
- (BOOL)approximateOnLiveResize;

- (BOOL)showCurve;
- (BOOL)showFill ;

- (NSColor *)curveColor;
- (NSColor *)fillColor ;

@end