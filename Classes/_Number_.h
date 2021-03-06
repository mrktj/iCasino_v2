//
//  Number.h
//  iCasinoCard
//
//  Created by Thanhhv on 5/9/14.
//

#ifndef __iCasinoCard__Number__
#define __iCasinoCard__Number__

#include "cocos2d.h"
#include "mUtils.h"

USING_NS_CC;
using namespace std;

class Number : public CCLayer
{
private:
    string numberString;
    string style;
    
    CCSize sizeThis;
    CCPoint pointStart;

public:
    Number(string pNumberString);
	Number(double pNumberDouble);
    ~Number();
    
    virtual bool init();
    
    void startRunAction();
    void startRunAction(float toY);

    CCSprite* getNumberWithChar(char a);
    void setNumbersWithString(string pNumberString);
    
    // setter, getter
    void setSize(int wid, int hei);
    CCSize getSize();
    void setPositionStart(CCPoint pPointStart);
	void setPositionStart(CCPoint pPointStart, float toY);

    CCPoint getPositionStart();
};

#endif /* defined(__iCasinoCard__Number__) */
