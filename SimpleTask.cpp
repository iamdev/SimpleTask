//***********************************************************************************
// MIT License
// 
// Copyright (c) 2018 Kamon Singtong
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//***********************************************************************************
// Library Name : SimpleTask for Arduino
// Architecture : Support All (AVR/ESP8266/STM32)
// Version : 1.0
// Owner : Kamon Singtong (MakeArduino.com)
// facebook : makearduino 
//***********************************************************************************

#include <Arduino.h>
#include "SimpleTask.h"

SimpleTask::SimpleTask(){}

task_t* SimpleTask::create(void (*callback)(task_t &),int interval)
{
    if(taskCount<MAX_TASK){
        struct task_t tk = {callback,NULL,interval,0,0,0,true}; 
        tasks[taskCount++] = tk;
        return &tk;
    }
} 

task_t* SimpleTask::create(void (*callback)(void),int interval)
{
    if(taskCount<MAX_TASK){
        struct task_t tk = {NULL,callback,interval,0,0,0,true}; 
        tasks[taskCount++] = tk;
        return &tk;
    }
}

void SimpleTask::loop(unsigned long t){    
    if(t==0) t = millis();        
    for(int i=0;i<taskCount;i++){
        if(!tasks[i].enabled)continue;
        unsigned long next = (tasks[i].timestamp + tasks[i].interval)&overflow_bitmask;        
        if(tasks[i].interval==0 || (t&overflow_bitmask)>=next){
            tasks[i].timestamp = t;            
            if(tasks[i].offset==0)tasks[i].offset = t-tasks[i].interval;
            tasks[i].timeoffset = t-tasks[i].offset;
            if(tasks[i].callback!=NULL)
                tasks[i].callback(tasks[i]);
            else 
                tasks[i].callback2();
        } 
    }   
}

SimpleTask Task;