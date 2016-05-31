#include "mainwindow.h"
#include <QApplication>

#include <libical/ical.h>
#include <libical/icalss.h>
#include <libical/icalfilesetimpl.h>

icalcomponent* fetch(icalset* set,const char* uid)
{
    icalfileset* fset = (icalfileset*)set;
    icalcompiter i;

    for(i = icalcomponent_begin_component(fset->cluster,ICAL_ANY_COMPONENT);
    icalcompiter_deref(&i)!= 0; icalcompiter_next(&i)){

        icalcomponent *_this = icalcompiter_deref(&i);
        icalcomponent *inner;
        icalproperty *p;
        const char *this_uid;

        for(inner = icalcomponent_get_first_component(_this,ICAL_ANY_COMPONENT);
            inner != 0;
            inner = icalcomponent_get_next_component(_this,ICAL_ANY_COMPONENT)){

            p = icalcomponent_get_first_property(inner,ICAL_UID_PROPERTY);
            if ( p )
            {
                this_uid = icalproperty_get_uid(p);

                if(this_uid==0){
                icalerror_warn("icalfileset_fetch found a component with no UID");
                continue;
                }

                if (strcmp(uid,this_uid)==0){
                    return inner;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
