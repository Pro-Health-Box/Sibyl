/*
   Copyright 2017 Hosang Yoon

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Simulation_data.h"

#include <fstream>

namespace sibyl
{

    /* ========================================== */
    /*                  KOSPISim                  */
    /* ========================================== */

bool KOSPISim::open(CSTR &path, CSTR &code)
{
    if (true != dataTr.open(path + code + STR(".txt" ))) return false;
    if (true != dataTb.open(path + code + STR("t.txt"))) return false;
    return true;
}

void KOSPISim::AdvanceTime(int timeTarget)
{
    dataTr.InitVecTr();
    dataTr.AdvanceTime(timeTarget);
    
    dataTb.AdvanceTime(timeTarget);
    tbr = dataTb.Tb();
    if (timeTarget > 0) Requantize(dataTr.TrPs1(), dataTr.TrPb1()); // after 09:00:00
    else                Requantize();
}

void KOSPISim::SetDelay(int d)
{
    dataTr.SetDelay(d);
    dataTb.SetDelay(d);
}


    /* ======================================== */
    /*                  ELWSim                  */
    /* ======================================== */

TxtDataTr ELWSim::dataKOSPI200Tr;
TxtDataTb ELWSim::dataKOSPI200Tb(SecType::KOSPI);

bool ELWSim::open(CSTR &path, CSTR &code)
{
    if (true != dataTr.open(path + code + STR(".txt" ))) return false;
    if (true != dataTb.open(path + code + STR("t.txt"))) return false;
    if (true != dataTh.open(path + code + STR("g.txt"))) return false;
    if ((dataKOSPI200Tr.is_open() == false) &&
        (true != dataKOSPI200Tr.open(path + STR("005930.txt" )))) return false;
    if ((dataKOSPI200Tb.is_open() == false) &&
        (true != dataKOSPI200Tb.open(path + STR("005930t.txt")))) return false;
    return true;
}

void ELWSim::AdvanceTime(int timeTarget)
{
    dataTr.InitVecTr();
    dataTr.AdvanceTime(timeTarget);
    
    dataTb.AdvanceTime(timeTarget);
    tbr = dataTb.Tb();
    if (timeTarget > 0) Requantize(dataTr.TrPs1(), dataTr.TrPb1()); // after 09:00:00
    else                Requantize();
    
    dataTh.AdvanceTime(timeTarget);
    for (std::size_t idx = 0; idx < szTh; idx++) thr[idx] = dataTh[idx + 1];
    
    dataKOSPI200Tr.InitVecTr();
    dataKOSPI200Tr.AdvanceTime(timeTarget);
    
    dataKOSPI200Tb.AdvanceTime(timeTarget);
    auto temp = dataKOSPI200Tb.Tb();
    if (timeTarget > 0) Requantize(temp, dataKOSPI200Tr.TrPs1(), dataKOSPI200Tr.TrPb1());
    else                Requantize(temp);
    
    kospi200 = TckLo(temp[idx::ps1].p) / 5000.0f;
}

void ELWSim::SetDelay(int d)
{
    dataTr.SetDelay(d);
    dataTb.SetDelay(d);
    dataTh.SetDelay(d);
    dataKOSPI200Tr.SetDelay(d);
    dataKOSPI200Tb.SetDelay(d);
}


    /* ========================================== */
    /*                   ETFSim                   */
    /* ========================================== */

bool ETFSim::open(CSTR &path, CSTR &code)
{
    if (true != dataTr .open(path + code + STR(".txt" ))) return false;
    if (true != dataTb .open(path + code + STR("t.txt"))) return false;
    if (true != dataNAV.open(path + code + STR("n.txt"))) return false;
    return true;
}

void ETFSim::AdvanceTime(int timeTarget)
{
    dataTr.InitVecTr();
    dataTr.AdvanceTime(timeTarget);
    
    dataTb.AdvanceTime(timeTarget);
    tbr = dataTb.Tb();
    if (timeTarget > 0) Requantize(dataTr.TrPs1(), dataTr.TrPb1()); // after 09:00:00
    else                Requantize();
    
    dataNAV.AdvanceTime(timeTarget);
    devNAV = (FLOAT) ((std::abs(dataNAV[1] / dataNAV[0]) - 1.0) * 100.0);
}

void ETFSim::SetDelay(int d)
{
    dataTr .SetDelay(d);
    dataTb .SetDelay(d);
    dataNAV.SetDelay(d);
}


}
