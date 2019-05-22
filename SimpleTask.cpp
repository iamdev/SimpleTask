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

//#define SIMPLETASK_DEBUG

SimpleTask::SimpleTask(){}

task_t* SimpleTask::create(void (*callback)(task_t &),int interval)
{
    if(taskCount<MAX_TASK){
        struct task_t tk = {callback,NULL,interval,true}; 
        tasks[taskCount++] = tk;
        return &tk;
    }
} 

task_t* SimpleTask::create(void (*callback)(void),int interval)
{
    if(taskCount<MAX_TASK){
        struct task_t tk = {NULL,callback,interval,true}; 
        tasks[taskCount++] = tk;
        return &tk;
    }
}
void SimpleTask::loop(){
    loop(millis());
}

void SimpleTask::loop(unsigned long t){    
    static bool first = true;
    for(int i=0;i<taskCount;i++){
        if(!tasks[i].enabled) continue;
        if(first){
            tasks[i].offset = t;
            tasks[i].next = t;
        }        
        t &= overflow_bitmask;
        if(t<=tasks[i].interval && tasks[i].next>overflow_bitmask){
            tasks[i].next &= overflow_bitmask;
        }
        #ifdef SIMPLETASK_DEBUG
        Serial.print("t="); 
        Serial.print(t);
        Serial.print("\tnext=");        
        Serial.println(tasks[i].next);
        #endif
        if(first || tasks[i].interval==0 || t>=tasks[i].next){
            unsigned long d = t - tasks[i].prev;            
            if(t<tasks[i].timestamp){
                d += overflow_bitmask+1; 
            }
            tasks[i].timestamp = t;                        
            if(tasks[i].offset==0)tasks[i].offset = t-tasks[i].interval;
                        
            tasks[i].timeoffset += d;
            if(first){
                tasks[i].offset = t;
                tasks[i].timeoffset -= tasks[i].offset; 
            }
            tasks[i].prev = t;
            tasks[i].next += tasks[i].interval;
            if(tasks[i].next<t) tasks[i].next = t+ tasks[i].interval;

            if(tasks[i].callback2!=NULL)
                tasks[i].callback2();
            else
                tasks[i].callback(tasks[i]); 
                
            #ifdef SIMPLETASK_DEBUG
            Serial.print("Event!! t=");
            Serial.println(tasks[i].timeoffset);
            #endif
        } 
    }   
    first = false;
}

SimpleTask Task;