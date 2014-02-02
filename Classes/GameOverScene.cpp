#include "GameOverScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

bool GameOverScene::init(){
	if(CCLayer::init()){
		CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
		this->_label=CCLabelTTF::create("2222","Î¢ÈíÑÅºÚ",32);
		this->initWithColor(ccc4(255,255,255,255));
		_label->retain();
		_label->setColor(ccc3(0,0,0));
		_label->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(_label);

		this->runAction(CCSequence::create(CCDelayTime::create(3),CCCallFunc::create(this,callfunc_selector(GameOverScene::gameOverDone)),NULL));
		return true;
	}else{
		return false;
	}
}

void GameOverScene::gameOverDone(){
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
GameOverScene::GameOverScene(void){
}
GameOverScene::~GameOverScene(void){
}