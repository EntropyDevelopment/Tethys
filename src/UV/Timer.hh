/*	Copyright 2017 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
	Distributed under the terms of the GNU Affero General Public License v3
*/

#if !defined ENTROPY_TETHYS_UV_TIMER_INC
#	define ENTROPY_TETHYS_UV_TIMER_INC

#	include <uv.h>
#	include <chrono>
#	include "Task.hh"

	extern "C" {
		void _entropy_tethys_uv_timer_cb(uv_timer_t *);
	}

	namespace Entropy
	{
		namespace Tethys
		{
			namespace UV
			{
				class Loop;

				class Timer :
					public Task
				{
					public:
						template<typename DelayType, typename RepeatType>
						Timer(const DelayType &, const RepeatType &, const std::function<void()> &);
						// 2018-02-01 AMR TODO: allow calling Timer(repeat, cb);
						virtual ~Timer();
						virtual void Added(Loop &);
						virtual void Stop();
					protected:
						template<typename DelayType, typename RepeatType>
						Timer(const DelayType &, const RepeatType &);
						template<typename RepeatType>
						explicit Timer(const RepeatType &);
					private:
						uv_timer_t _handle;
						std::chrono::duration<std::uint64_t, std::ratio<1,1000>> _delay;
						std::chrono::duration<std::uint64_t, std::ratio<1,1000>> _repeat;
					friend void ::_entropy_tethys_uv_timer_cb(uv_timer_t *);
				};
			}
		}
	}

#	include "Loop.hh"

#	include "Timer.impl.hh"

#endif
