//
//  ViewController.m
//  iDrinkWater
//
//  Created by Max Kievits on 12/06/2019.
//  Copyright © 2019 MaxMedia-Apps. All rights reserved.
//

#import "ViewController.h"

@interface ViewController () {
    NSInteger desiredXPos;
    float waterLevelPercentage;
    float waterDrank;
    
    int oldML;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    //720
    
    waterDrank = 0;
    
    [self downloadData];
    
    [self setNeedsStatusBarAppearanceUpdate];
    
    waterView1 = [[UIImageView alloc] initWithFrame:CGRectMake(0, 200, self.view.frame.size.width, 720)];
    waterView2 = [[UIImageView alloc] initWithFrame:CGRectMake(self.view.frame.size.width, 200, self.view.frame.size.width, 720)];
    
    waterView1.image = [UIImage imageNamed:@"AnimatingWater"];
    waterView2.image = [UIImage imageNamed:@"AnimatingWater"];
    
    [self.view addSubview:waterView1];
    [self.view addSubview:waterView2];
    
    [self.view sendSubviewToBack:waterView1];
    [self.view sendSubviewToBack:waterView2];
    
    [NSTimer scheduledTimerWithTimeInterval:2 repeats:true block:^(NSTimer * _Nonnull timer) {
        [self downloadData];
    }];


    [NSTimer scheduledTimerWithTimeInterval:0.01 repeats:true block:^(NSTimer * _Nonnull timer) {
        self->waterView1.center = CGPointMake(self->waterView1.center.x - 1, self->waterView1.center.y);
        self->waterView2.center = CGPointMake(self->waterView2.center.x - 1, self->waterView2.center.y);

        if (self->waterView1.frame.origin.x < -self.view.frame.size.width) {
            self->waterView1.frame = CGRectMake(self.view.frame.size.width-1, self->waterView1.frame.origin.y, self.view.frame.size.width, 720);
        }
        
        if (self->waterView2.frame.origin.x < -self.view.frame.size.width) {
            self->waterView2.frame = CGRectMake(self.view.frame.size.width-1, self->waterView2.frame.origin.y, self.view.frame.size.width, 720);
        }
        
        //NSLog(@"%f - %ld",self->waterView1.center.y, (long)self->desiredXPos);
        
        int increment = 0;
        if (self->desiredXPos < self->waterView1.center.y) {
            increment = -1;
        } else if (self->desiredXPos > self->waterView1.center.y) {
            increment = 1;
        }
        
        if (self->waterView1.center.y == self->desiredXPos) {
            increment = 0;
        }
    
        
        if (self->waterView1.center.y != self->desiredXPos) {
            self->waterView1.center = CGPointMake(self->waterView1.center.x, self->waterView1.center.y + increment);
            self->waterView2.center = CGPointMake(self->waterView2.center.x, self->waterView2.center.y + increment);
        } else {
            self->waterView1.center = CGPointMake(self->waterView1.center.x, self->desiredXPos);
            self->waterView2.center = CGPointMake(self->waterView2.center.x, self->desiredXPos);
        }
        
        [self updateLabelAppearence];
    }];
    
    //https://max.mobidapt.com/ixd/getWaterData.php?thing_id=1
}

-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self updateLabelAppearence];
}

-(void)downloadData {
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
        //https://walibiapp.com/images/<IMAGE>
        
        NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://max.mobidapt.com/ixd/getWaterData.php?thing_id=1"]];
        
        //create the task
        NSURLSessionDataTask* task = [[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
            dispatch_async(dispatch_get_main_queue(), ^{
                
                if (!(data == nil)) {
                    // Parse the JSON that came in
                    NSError *error;
                    
                    NSArray *newJsonArray = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
                    NSString *waterLevelString = newJsonArray.lastObject[@"waterLevel"];
                    
                    self->waterLevelPercentage = [waterLevelString floatValue];
                    
                    self->mlWaterLabel.text = [NSString stringWithFormat:@"%ldml",(long)[self calculateMililetersLeftWithPercentage:[waterLevelString intValue]]];
                    
                    self->percentWaterLabel.text = [NSString stringWithFormat:@"%@%%",waterLevelString];
                    
                    //[UIView animateWithDuration:0 animations:^{
                        //54 - 697
                    
                    self->desiredXPos = (697 + 348) - (0 + ((643) * ([waterLevelString floatValue]/100)));

                    float currentML = [self calculateMililetersLeftWithPercentage:[waterLevelString intValue]];
                    if (self->oldML < currentML) {
                        float difference = self->oldML - currentML;
                        
                        self->waterDrank = self->waterDrank + difference;
                        
                        [self updateProgressWithTotalWaterDrank:self->waterDrank];
                        
                        self->oldML = currentML;
                    }
                    
                    NSLog(@"%f",self->waterDrank);
                    
                    /*
                    self->waterView1.center = CGPointMake(self->waterView1.center.x, (697 + 348) - (54 + (643 * ([waterLevelString floatValue]/100))));
                    self->waterView2.center = CGPointMake(self->waterView2.center.x, (697 + 348) - (54 + (643 * ([waterLevelString floatValue]/100))));
                     */
                    
                    //[self updateLabelAppearence];
                    
                        //self->waterView1.frame = CGRectMake(self->waterView1.center.x, 697 - (54 + (643 * ([waterLevelString floatValue]/100))), self->waterView1.frame.size.width, self->waterView1.frame.size.height);
                       // self->waterView2.frame = CGRectMake(self->waterView2.center.x, 697 - (54 + (643 * ([waterLevelString floatValue]/100))), self->waterView2.frame.size.width, self->waterView2.frame.size.height);

                    //}];
                }
            });
            
        }];
        
        [task resume];
    });
}

-(void)updateLabelAppearence {
    for (UILabel *label in self.view.subviews) {
        if (label.tag == 20) {
            if (label.frame.origin.y > (self->waterView1.frame.origin.y + 20)) {
                [label setTextColor:[UIColor whiteColor]];
            } else {
                [label setTextColor:[UIColor blackColor]];
            }
        }
    }
    
    
}

-(void)updateProgressWithTotalWaterDrank:(float)waterDrank {
    NSLog(@"%f",self->waterDrank);
    self->progressBar.frame = CGRectMake(self->progressBar.frame.origin.x, self->progressBar.frame.origin.y, 200, 20);
}

-(NSInteger)calculateMililetersLeftWithPercentage:(float)percentage {
    return 1000-(1000*(percentage/100));
}

-(UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

@end
