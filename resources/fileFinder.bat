@Echo Off & Title 
Echo Status oparations: In Progress...
For /R . %%i In (*.*) Do (
 	If Exist %%i (
		Echo %%i >> res.txt
	)
)
Cls & Echo Status Oparations: Ended Seacher
Pause & Start resource.txt & Echo On