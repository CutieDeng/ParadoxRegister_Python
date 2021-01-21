#!/usr/bin/env python
# -*- coding:UTF-8 -*-
import sys
import os
import ctypes
import shutil

from ctypes.wintypes import MAX_PATH

def check_contain_chinese(check_str):
    for ch in check_str:
        if u'\u4e00' <= ch <= u'\u9fff':
            return True
    return False

number = 0
modlist = []
foundError = 'NoFound'
def getDocuments():
    dll = ctypes.windll.shell32
    buf = ctypes.create_unicode_buffer(MAX_PATH + 1)
    if dll.SHGetSpecialFolderPathW(None, buf, 0x0005, False):
        #请在这里替换Hearts of Iron IV为其他的值。
        return buf.value + "\\Paradox Interactive\\Hearts of Iron IV\\mod\\"
    else:
        return foundError

def registermod():
    global number
    global modlist
    filepath = "mod" # 设置检查文件夹为mod文件夹
    if not os.path.exists(filepath): # 判断mod文件夹是否存在
        print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
        print("！！！！！！！！！！！！抱歉您的mod文件夹不存在，请检查或者创建！！！！！！！！！！！！")
        print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
        os.system('pause')
    for modname in os.listdir(filepath):
        if os.path.isdir(filepath + "\\" + modname):
            #path用于生成游戏需要的位置
            path = (os.getcwd() + "\\mod\\" + modname).replace("\\","/")
            #检查模组目录是否包含中文，如果有，提示修改。
            if check_contain_chinese(path):
                print("\n您的模组文件目录:" + path + "包含中文，请修改，否则注册不会生效，该模组已经自动不注册！\n")
            else:
                needname = os.getcwd() + "\\mod\\" + modname + "\\descriptor.mod"
            
                if not os.path.exists(needname):
                    print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
                    print("！！！！！！！！您的" + os.getcwd() + "\\mod\\" + modname + "这个模组识别文件不存在，请检查模组文件是否正确！！！！！！！！")
                    print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
                else:
                    cunfang = open(needname,"r")
                    checkline = cunfang.readlines()
                    for lines in checkline:
                        if "name=" in lines:
                            #print(lines[6:-2])
                            modname = lines[6:-2]# 得到了模组的名字
                            if not os.path.exists(getDocuments()):
                                print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
                                print("！！！！！！！！看来您是第一次启动游戏和启动器，请重启启动器，保证注册生效哦。！！！！！！！！！")
                                print("——————————————————————————————————————————————————————————————————————————————————————————————————————————————————")
                                os.system('pause')
                            xinjian = open(getDocuments() + modname +".mod",mode='w+')
                        
                            xinjian.write(lines +  "path="+ "\""  + path + "\"")
                            #生成了一个模组。
                            number = number + 1
                            modlist.append(modname)
                        
                #关闭两个文件
                cunfang.close()
                xinjian.close()
def display():
    global number
    global modlist
    print("√√√√√√√√√√√√√√√√√您注册了" + str(number) + "个模组,他们名字是：√√√√√√√√√√√√√√√√√√√√√√")
    for temp in modlist:
          print(temp)
def runlauncher():
    print("☆您好！这里是Pinenut的模组注册工具！Python编写的~")
    print("☆该工具授权雪栗星貉整合入整合包~（实际上就是帮他写的.jpg)")
    print("☆接下来会启动启动器，在这个框下会显示新注册的模组！")
    print("☆只要把模组的文件夹放到mod文件夹内就好了哦！（暂时还没有支持压缩包模组）")
    #这句话本身是用来启动启动器的，现在暂时注释掉。
    #os.popen("launcher\\Paradox Launcher.exe")
    print("启动器已尝试启动，正在注册模组！")

if __name__ == "__main__":
    runlauncher()
    registermod()
    display()
    os.system('pause')
    
