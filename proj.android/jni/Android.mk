LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/MainScene.cpp \
				   ../../Classes/CloseScene.cpp \
				   ../../Classes/SettingsScene.cpp \
				   ../../Classes/EnterPlayerScene.cpp \
				   ../../Classes/LeaderboardScene.cpp \
				   ../../Classes/GameScene/GameScene.cpp \
				   ../../Classes/GameScene/Bullet.cpp \
				   ../../Classes/GameScene/Enemy.cpp \
				   ../../Classes/GameScene/GameMenuScene.cpp \
				   ../../Classes/GameScene/GameManager.cpp \
				   ../../Classes/GameScene/Player.cpp \
				   ../../Classes/GameScene/Bonus.cpp \
				   ../../Classes/GameScene/LevelCompleteScene.cpp \
				   ../../Classes/GameSettings.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
				   ../../Classes/GameScene \
				   ../../Classes/rapidjson

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
