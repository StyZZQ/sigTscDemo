#include "cycle.h"

Cycle::Cycle() 
{
    initAll(all);
	bakAll = all;
}

void Cycle::initAll(All& a)
{
	Tscdb   db;
	if (db.Open()) 	
		return;
	db.Load(all.device.conf,"device");
	db.Load(all.basic.conf,"basic");
	db.Load(all.light.conf,"light");
	db.Load(all.phase.conf,"phase");
	db.Load(all.detector.conf,"detector");
	db.Load(all.PhaseStage.conf,"phaseStage");
	db.Load(all.phaseSCY.conf,"phaseSCY");
	db.Load(all.emrgPrior.conf,"emrgPrior");
	db.Load(all.plan.conf,"plan");
	db.Load(all.transition.conf,"transition");
	db.Load(all.scheme.conf,"scheme");
	db.Load(all.schedule.conf,"schedule");
	db.Load(all.runningStatus.conf,"runningStatus");
	db.Load(all.traffic.conf,"traffic");
	db.Load(all.alarm.conf,"alarm");
	db.Load(all.fault.conf,"fault");
	db.Load(all.centerCtrl.conf,"centerCtrl");
	db.Load(all.order.conf,"orders");      //order在sql中是关键字,所以表名用orders
	db.Close();	
}
void Cycle::UploadTraffic()
{
	Frame frame;
	frame.send();	
}
void Cycle::UploadAlarm()
{
	Frame frame;
	frame.send();
}
void Cycle::UploadFault()
{
	Frame frame;
	frame.send();
}
void Cycle::GetLocalRule()
{

}

void Cycle::run(void *arg)
{

}