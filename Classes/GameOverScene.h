#ifndef _GAMEOVER_SCENE_H__
#define _GAMEOVER_SCENE_H__
#include "cocos2d.h"
class GameOverScene :public cocos2d::CCLayerColor
{
public:
	GameOverScene(void);
	~GameOverScene(void);
	virtual bool init();
	void gameOverDone();
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*,_label,Label);
	CREATE_FUNC(GameOverScene);
};

#endif // _GAMEOVER_SCENE_H__
