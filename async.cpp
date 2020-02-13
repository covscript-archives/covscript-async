#include <covscript/dll.hpp>
#include <covscript/cni.hpp>
#include <process.hpp>
#include <future>

using covscript_process::process_t;
using covscript_process::process_builder;

cs::var wait_for_impl(std::size_t mill_sec, const cs::callable &func, cs::vector &args)
{
    std::future<cs::var> future = std::async(std::launch::async, [&func, &args]{
        return func.call(args);
    });
    if (future.wait_for(std::chrono::milliseconds(mill_sec)) != std::future_status::ready)
        throw cs::lang_error("Target function deferred or timeout.");
    else
        return future.get();
}

cs::var wait_until_impl(std::size_t mill_sec, const cs::callable &func, cs::vector &args)
{
    std::future<cs::var> future = std::async(std::launch::async, [&func, &args]{
        return func.call(args);
    });
    if (future.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(mill_sec)) != std::future_status::ready)
        throw cs::lang_error("Target function deferred or timeout.");
    else
        return future.get();
}

cs::var wait_for(std::size_t mill_sec, const cs::var &func, const cs::array &argument)
{
    if (func.type() == typeid(cs::callable)) {
        cs::vector args(argument.begin(), argument.end());
        return wait_for_impl(mill_sec, func.const_val<cs::callable>(), args);
    }
    else if (func.type() == typeid(cs::object_method)) {
        const auto &om = func.const_val<cs::object_method>();
        cs::vector args{om.object};
        args.insert(args.end(), argument.begin(), argument.end());
        return wait_for_impl(mill_sec, om.callable.const_val<cs::callable>(), args);
    }
    else
        throw cs::lang_error("Invoke non-callable object.");
}

cs::var wait_until(std::size_t mill_sec, const cs::var &func, const cs::array &argument)
{
    if (func.type() == typeid(cs::callable)) {
        cs::vector args(argument.begin(), argument.end());
        return wait_until_impl(mill_sec, func.const_val<cs::callable>(), args);
    }
    else if (func.type() == typeid(cs::object_method)) {
        const auto &om = func.const_val<cs::object_method>();
        cs::vector args{om.object};
        args.insert(args.end(), argument.begin(), argument.end());
        return wait_for_impl(mill_sec, om.callable.const_val<cs::callable>(), args);
    }
    else
        throw cs::lang_error("Invoke non-callable object.");
}

CNI_ROOT_NAMESPACE {
	CNI_NAMESPACE(process)
	{
		CNI_TYPE_EXT(builder, process_builder, process_builder()) {
			CNI_V(redirect_stdin, &process_builder::redirect_stdin)
			CNI_V(redirect_stdout, &process_builder::redirect_stdout)
			CNI_V(redirect_stderr, &process_builder::redirect_stderr)
			CNI_V(start, &process_builder::start)
		}

		CNI_NAMESPACE(process) {
			CNI_V(in, &process_t::get_cs_stdin)
			CNI_V(out, &process_t::get_cs_stdout)
			CNI_V(err, &process_t::get_cs_stderr)
			CNI_V(wait, &process_t::wait_for_exit)
			CNI_V(exit_code, &process_t::get_exit_code)
			CNI_V(has_exited, &process_t::has_exited)
		}
	}
	CNI_NAMESPACE(future)
	{
		CNI(wait_for)
		CNI(wait_until)
	}
}

CNI_ENABLE_TYPE_EXT_V(process::process, process_t, process)
CNI_ENABLE_TYPE_EXT_V(process::builder, process_builder, process::builder)