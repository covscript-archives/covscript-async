import async.future as future
var ts = null
ts = runtime.time()
future.wait_for  (1000, [](time)->runtime.delay(time), {500})
system.out.println(runtime.time() - ts)
ts = runtime.time()
future.wait_until(1000, [](time)->runtime.delay(time), {500})
system.out.println(runtime.time() - ts)