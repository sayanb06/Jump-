# Devlopment Progress
### 4/21:
#### created platforms and displayed random platforms (new one each time draw was called)

## 4/26:
### 1. created person, and displayed person
### 2. added marker where mouse is, and dotted lines from right arm to the marker
### 3. drew Professor Carl Graham Evans!
### 4. Added sound

## 4/27:
### 1. added motion when the user clicks it's not going in the correct direction
### 2. motion is fixed including the following bugs: multiples jumps, switching direction while in the middle of the path. Have to add multiple platforms capability
### 3. Full motion including multiple platforms, infinite scroll, and intersection
A bug that persists is intersections between the head and the next platform

## 4/28
### 1. Added Menu Support:
added q option to exit game, but currently nothing is shown for High Scores, or Paused

## 4/30
### 1. Cleaned up code
found most space/time effective solutions to storing last clicked location  
explained code with comments, formatted code, and removed magic numbers, started keeping track of high scores  
Issues: Platforms moving around and smaller platforms haven't been implemented, and when the user is in the middle of switching platforms, the ball can turn around  

## 5/1
## 1. Circle and Platform Intersecting
I tried redefining how to know if a platform is intersecting with the head but it didn't work nearly as well as I'd have hoped
## Game Over
I handled game over by making sure correct scenario was shown.
## High Scores
I just displayed high scores when the game was over and in the menu by choice. Then when closing by 'Q', I made sure the high scores were added to the local file.
## drawPause
I made a pause screen.  

## To finish up the project:
1. I want to see if I can add stars  
2. I need to have the platform getting smaller as time goes on
3. I also need to make sure the head of Prof. Evans looks circular rather than rectangular.
4. I want to add a control to the high scores screen so that the high scores can be cleared and given a fresh start
5. Finally, I just need to clean up my code to make sure that it is readable.