//
//  _Avatar_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/15/14.
//  4 avatar for game
//
// // parent->this===>setListUserBy...->formatAndStore->updateUsers.
//

#ifndef __iCasino_v2___Avatar_inTomCuaCa___
#define __iCasino_v2___Avatar_inTomCuaCa___

#include <iostream>

#include "cocos2d.h"
//
#include "AllData.h"
#include "_Avatar_.h"

using namespace cocos2d;

class AvatarInTomCuaCa : public CCLayer
{
public:
	~AvatarInTomCuaCa();
	virtual void onExit();
	virtual bool init();
	CREATE_FUNC(AvatarInTomCuaCa);

	void setIcon (int pos, char *url);
	void setFlag (int pos, bool isShow);
	void setName (int pos, string name);
	void setMoney(int pos, string money);
	void setMoney(int pos, int money);
	void setMoney(int pos, double money);
	void setReady(int pos, bool isReady);
	void setVisibleLayerInvite(int pos, bool isShow);

	void resetAll();
	void setPosChuong(int pos);

	void setUnReadyAllUser();
	void resetGame();
    UIButton *btn_vaochoi;
	UIButton *btn_dungday;

	//

	Avatar* getUserByPos(int pos);
	int getIndexInArrByName(string name);
	int getIndexInArrByAccountID(string aI);
	
	int getPosByAccountID(string aI);
	int getPosByName(string name);
	string getNameByPos(int pos);
	string getAccountIDByPos(int pos);
	void updateUsers();
	void showChatByPos(int pos, string mes);
	// Time
    bool isSpect();
    bool isStartedGame();
    void vaoBanChoi(CCObject *obj,TouchEventType type);
    void playerToSpec();
    void specToPlayer();
    void setListUser(string list);
private:


	int typeGame;
	bool isGuess;
	string myName;
	string listUser;
	string myAI;

	// mảng chứa
	vector<string> arrName;
	vector<string> arrFlag;
	vector<string> arrURL;
	vector<string> arrAI;
	vector<int> arrMoney;
	vector<double> arrMoneyDouble;
	UIImageView *chuong;
};

#endif /* defined(__iCasino_v2___Avatar_inGame___) */
