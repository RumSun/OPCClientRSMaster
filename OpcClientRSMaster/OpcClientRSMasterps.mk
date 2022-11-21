
OpcClientRSMasterps.dll: dlldata.obj OpcClientRSMaster_p.obj OpcClientRSMaster_i.obj
	link /dll /out:OpcClientRSMasterps.dll /def:OpcClientRSMasterps.def /entry:DllMain dlldata.obj OpcClientRSMaster_p.obj OpcClientRSMaster_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del OpcClientRSMasterps.dll
	@del OpcClientRSMasterps.lib
	@del OpcClientRSMasterps.exp
	@del dlldata.obj
	@del OpcClientRSMaster_p.obj
	@del OpcClientRSMaster_i.obj
