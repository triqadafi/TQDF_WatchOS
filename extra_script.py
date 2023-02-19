import os
Import("env")
use_precompiled = env.GetProjectOption("use_precompiled")
if(use_precompiled == "true"):
  # env.Append(BUILD_FLAGS=["-Llib/TQDF_WatchOS_STM32/src/"+env.GetProjectOption("cpu_core"), "-lSTM32duinoLowPower"])
  env.ProcessFlags("-L"+os.path.join(os.getcwd(), "src",env.GetProjectOption("cpu_core")).replace('\\','\\\\'))
  folder_name = os.path.basename(os.getcwd())
  env.ProcessFlags("-l"+folder_name)
  # print(env.Dump())
  # print("File location using os.getcwd():", os.path.join(os.getcwd(), "src",env.GetProjectOption("cpu_core")).replace('\\','\\\\'))


