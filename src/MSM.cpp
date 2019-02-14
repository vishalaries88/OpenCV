#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;

namespace
{
    // events
    struct recieved_connection {};
    struct rot_mot {};
    struct stop_mot {};
    struct reset_system {};

    // A "complicated" event type that carries some data.
	enum DiskTypeEnum
    {
        DISK_CD=0,
        DISK_DVD=1
    };
    struct cd_detected
    {
        cd_detected(std::string name, DiskTypeEnum diskType)
            : name(name),
            disc_type(diskType)
        {}

        std::string name;
        DiskTypeEnum disc_type;
    };

    // front-end: define the FSM structure 
    struct tsif : public msm::front::state_machine_def<tsif>
    {
        template <class Event,class FSM>
        void on_entry(Event const& ,FSM&) 
        {
            std::cout << "entering: tsif" << std::endl;
        }
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) 
        {
            std::cout << "leaving: tsif" << std::endl;
        }

        // The list of FSM states
        struct Idle : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "entering: Idle" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: Idle" << std::endl;}
        };
        struct Connected : public msm::front::state<> 
        {	 
            template <class Event,class FSM>
            void on_entry(Event const& ,FSM&) {std::cout << "entering: Connected" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: Connected" << std::endl;}
        };

        // sm_ptr still supported but deprecated as functors are a much better way to do the same thing
        struct Stopped : public msm::front::state<msm::front::default_base_state,msm::front::sm_ptr> 
        {	 
            template <class Event,class FSM>
            void on_entry(Event const& ,FSM&) {std::cout << "entering: Stopped" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: Stopped" << std::endl;}
            void set_sm_ptr(tsif* pl)
            {
                m_player=pl;
            }
            tsif* m_player;
        };

        struct Rotating : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {std::cout << "entering: Rotating" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: Rotating" << std::endl;}
        };

        // state not defining any entry or exit
        struct Paused : public msm::front::state<>
        {
        };

        // the initial state of the player SM. Must be defined
        typedef Idle initial_state;

        // transition actions
        void establish_connection(recieved_connection const&)       { std::cout << "tsif::Setting up the connection\n"; }
        void start_rotation(rot_mot const&)    { std::cout << "tsif::Starting the rotation\n"; }
        void stop_rotation(stop_mot const&)   { std::cout << "tsif::Stopping the  rotation\n"; }
        void finishing(reset_system const&) { std::cout << "tsif:: Resetting the tsif to connected\n"; }
        // void stop_playback(stop const&)        { std::cout << "player::stop_playback\n"; }
        // void pause_playback(pause const&)      { std::cout << "player::pause_playback\n"; }
        // void resume_playback(end_pause const&)      { std::cout << "player::resume_playback\n"; }
        // void stop_and_open(open_close const&)  { std::cout << "player::stop_and_open\n"; }
        // void stopped_again(stop const&)	{std::cout << "player::stopped_again\n";}
        // guard conditions
        bool good_disk_format(cd_detected const& evt)
        {
            // to test a guard condition, let's say we understand only CDs, not DVD
            if (evt.disc_type != DISK_CD)
            {
                std::cout << "wrong disk, sorry" << std::endl;
                return false;
            }
            return true;
        }
        // used to show a transition conflict. This guard will simply deactivate one transition and thus
        // solve the conflict
        bool auto_start(cd_detected const&)
        {
            return false;
        }

        typedef tsif p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event                      Next                    Action			   Guard
            //  +---------+--------  -----------+-----------------+--------------------    -+----------------------+
          a_row < Idle , recieved_connection        , Connected , &p::establish_connection                        >,
            //  +---------+-------------+---------+---------------------+----,------------------+
          a_row < Connected    , rot_mot  , Rotating   , &p::start_rotation                       >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Rotating   , stop_mot  , Stopped    , &p::stop_rotation                            >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Stopped , reset_system        , Connected , &p::finishing                        >
        > {};
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    // Pick a back-end
    typedef msm::back::state_machine<tsif> tsif_;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Idle", "Connected", "Rotating", "Stopped"};
    void pstate(tsif_ const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
		tsif_ p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
         p.start(); 
        // // go to Open, call on_exit on Empty, then action, then on_entry on Open
        p.process_event(recieved_connection()); pstate(p);
        p.process_event(rot_mot()); pstate(p);
        // will be rejected, wrong disk type
        p.process_event(stop_mot()); pstate(p);
        p.process_event(reset_system()); pstate(p);
		// p.process_event(play());

        // // at this point, Play is active      
        // p.process_event(pause()); pstate(p);
        // // go back to Playing
        // p.process_event(end_pause());  pstate(p);
        // p.process_event(pause()); pstate(p);
        // p.process_event(stop());  pstate(p);
        // // event leading to the same state
        // // no action method called as it is not present in the transition table
        // p.process_event(stop());  pstate(p);
         std::cout << "stop fsm" << std::endl;
         p.stop();
    }
}

int main()
{
    test();
    return 0;
}