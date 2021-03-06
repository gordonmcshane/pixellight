//Create the chmfile object for this HTML page
var chmfile = new Object();
chmfile.fullname = "";
chmfile.name = "";
chmfile.path = "";
chmfile.page = "";
getchmfile();

// Get the path and name of the CHM file and assign object values
function getchmfile() {
	var ra, sl, a, X, Y, Z;
	ra = /:/;
	a = location.href.search(ra);
	sl = "::";
	Y = location.href.lastIndexOf(sl);
	if (a == 4) { // file: or http:
		Y = location.href.lastIndexOf("/");
		chmfile.page = unescape(location.href.substring(Y+1));
		if (location.href.substring(0,4) == "file") {
			chmfile.path = unescape(location.href.substring(8, Y+1));
		}
	} else {
		if (a == 2) X = 14; // mk:@MSITStore:
		if (a == 7) X = 7; // ms-its:
		chmfile.fullname = unescape(location.href.substring(X, Y));
		Z = chmfile.fullname.lastIndexOf("\\");
		chmfile.path = unescape(chmfile.fullname.substring(0, Z+1));
		chmfile.name = unescape(chmfile.fullname.substring(Z+1));
		chmfile.page = unescape(location.href.substring(Y+3));
	}
	/*
	alert(location.href+"\n\n"+"fullname="+chmfile.fullname+"\n"+"path="+chmfile.path+"\n"+"name="+chmfile.name+"\n"+"page="+chmfile.page);
	*/
}

// Write hidden shortcut link object in JavaScript
function shortcut(shortcut_id, shortcut_path) {
	document.write('<OBJECT id='+shortcut_id+' type="application/x-oleobject"');
	document.write(' classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"');
	document.write(' codebase="hhctrl.ocx"');
	document.write(' >');
	document.write(' <PARAM name="Command" value="ShortCut">');
	document.write(' <PARAM name="Item1" value=",'+'&quot;'+chmfile.path+shortcut_path+'&quot;'+',">');
	document.write('</OBJECT>');
}

function text_shortcut(shortcut_id, shortcut_path) {
	document.write('<OBJECT id='+shortcut_id+' type="application/x-oleobject"');
	document.write(' classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"');
	document.write(' codebase="hhctrl.ocx"');
	document.write(' >');
	document.write(' <PARAM name="Command" value="ShortCut">');
	document.write(' <PARAM name="Item1" value=",WordPad.exe,'+'&quot;'+chmfile.path+shortcut_path+'&quot;'+'">');
	document.write('</OBJECT>');
}

function scene_exe_shortcut(shortcut_id, shortcut_path) {
	var exe = chmfile.path+"Tools\\x86\\PLViewer.exe";
	var scene = '&quot;'+chmfile.path+shortcut_path+'&quot;';
	document.write('<OBJECT id='+shortcut_id+' type="application/x-oleobject"');
	document.write(' classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"');
	document.write(' codebase="hhctrl.ocx"');
	document.write(' >');
	document.write(' <PARAM name="Command" value="ShortCut">');
	document.write(' <PARAM name="Item1" value=",'+exe+','+scene+'">');
	document.write('</OBJECT>');
}


// License text shortcuts
text_shortcut("License_cc_by_nc_sa_3",	"cc-by-nc-sa-3.txt");


// Documentation shortcuts
shortcut("Doc_ReleaseNotes",				"ReleaseNotes.txt");
shortcut("Doc_PixelLightFeatures",			"Docs\\PixelLightFeatures.pdf");
shortcut("Doc_PixelLightBase",				"Docs\\PixelLightBase.pdf");
shortcut("Doc_PixelLightAPI",				"Docs\\PixelLightAPI.chm");
shortcut("Doc_PixelLightCompositing",		"Docs\\PixelLightCompositing.pdf");
shortcut("Doc_PixelLightConventions",		"Docs\\PixelLightConventions.pdf");
shortcut("Doc_PixelLightBuild",				"Docs\\PixelLightBuild.pdf");
shortcut("Doc_PixelLightScript",			"Docs\\PixelLightScript.pdf");
shortcut("Doc_PixelLight3dsMaxSceneExport",	"Docs\\PixelLight3dsMaxSceneExport.pdf");


// Directory shortcuts
// Misc
shortcut("Dir_Docs",					"Docs\\");
shortcut("Dir_Lib",						"Lib\\");
shortcut("Dir_Runtime",					"Runtime\\");
// Tools
shortcut("Dir_Tools",					"Tools\\");
shortcut("Dir_3dsMax",					"Tools\\3dsMax\\");
// include
shortcut("Dir_Include",					"Include\\");
shortcut("Dir_PLCore",					"include\\PLCore\\");
shortcut("Dir_PLMath",					"include\\PLMath\\");
shortcut("Dir_PLGraphics",				"include\\PLGraphics\\");
shortcut("Dir_PLDatabase",				"include\\PLDatabase\\");
shortcut("Dir_PLIRC",					"include\\PLIRC\\");
shortcut("Dir_PLJabber",				"include\\PLJabber\\");
shortcut("Dir_PLGui",					"include\\PLGui\\");
shortcut("Dir_PLInput",					"include\\PLInput\\");
shortcut("Dir_PLSound",					"include\\PLSound\\");
shortcut("Dir_PLRenderer",				"include\\PLRenderer\\");
shortcut("Dir_PLMesh",					"include\\PLMesh\\");
shortcut("Dir_PLScene",					"include\\PLScene\\");
shortcut("Dir_PLPhysics",				"include\\PLPhysics\\");
shortcut("Dir_PLEngine",				"include\\PLEngine\\");
shortcut("Dir_PLParticleGroups",		"include\\PLParticleGroups\\");
shortcut("Dir_SPARK_PL",				"include\\SPARK_PL\\");
shortcut("Dir_PLlibRocket",				"include\\PLlibRocket\\");
// Samples
shortcut("Dir_Samples",					"Samples\\");
shortcut("Dir_Samples_Bin",				"Samples\\Bin\\");
shortcut("Dir_Sample_Scenes",			"Samples\\Bin\\Data\\Scenes\\");
shortcut("Dir_05NetworkChat",			"Samples\\05NetworkChat\\");
shortcut("Dir_45ScriptApplication",		"Samples\\45ScriptApplication\\");
shortcut("Dir_50RendererTriangle",		"Samples\\50RendererTriangle\\");
shortcut("Dir_51RendererTexturing",		"Samples\\51RendererTexturing\\");
shortcut("Dir_52RendererMesh",			"Samples\\52RendererMesh\\");
shortcut("Dir_53RendererMultiView",		"Samples\\53RendererMultiView\\");
shortcut("Dir_54RendererRTT",			"Samples\\54RendererRTT\\");
shortcut("Dir_55RendererTessellation",	"Samples\\55RendererTessellation\\");
shortcut("Dir_56RendererInstancing",	"Samples\\56RendererInstancing\\");
shortcut("Dir_60Scene",					"Samples\\60Scene\\");
shortcut("Dir_61Sound",					"Samples\\61Sound\\");
shortcut("Dir_62Physics",				"Samples\\62Physics\\");
shortcut("Dir_63PhysicsRagdoll",		"Samples\\63PhysicsRagdoll\\");
shortcut("Dir_64Picking",				"Samples\\64Picking\\");
shortcut("Dir_65PostProcess",			"Samples\\65PostProcess\\");
shortcut("Dir_66IngameGUI",				"Samples\\66IngameGUI\\");
shortcut("Dir_67Game2D",				"Samples\\67Game2D\\");
shortcut("Dir_70SPARK",					"Samples\\70SPARK\\");
shortcut("Dir_71libRocket",				"Samples\\71libRocket\\");


// Executable shortcuts
// Samples
shortcut("Exe_05NetworkChat",			"Samples\\Bin\\x86\\05NetworkChat.exe");
shortcut("Exe_45ScriptApplication",		"Samples\\Bin\\x86\\45ScriptApplication.exe");
shortcut("Exe_50RendererTriangle",		"Samples\\Bin\\x86\\50RendererTriangle.exe");
shortcut("Exe_51RendererTexturing",		"Samples\\Bin\\x86\\51RendererTexturing.exe");
shortcut("Exe_52RendererMesh",			"Samples\\Bin\\x86\\52RendererMesh.exe");
shortcut("Exe_53RendererMultiView",		"Samples\\Bin\\x86\\53RendererMultiView.exe");
shortcut("Exe_54RendererRTT",			"Samples\\Bin\\x86\\54RendererRTT.exe");
shortcut("Exe_55RendererTessellation",	"Samples\\Bin\\x86\\55RendererTessellation.exe");
shortcut("Exe_56RendererInstancing",	"Samples\\Bin\\x86\\56RendererInstancing.exe");
shortcut("Exe_60Scene",					"Samples\\Bin\\x86\\60Scene.exe");
shortcut("Exe_61Sound",					"Samples\\Bin\\x86\\61Sound.exe");
shortcut("Exe_62Physics",				"Samples\\Bin\\x86\\62Physics.exe");
shortcut("Exe_63PhysicsRagdoll",		"Samples\\Bin\\x86\\63PhysicsRagdoll.exe");
shortcut("Exe_64Picking",				"Samples\\Bin\\x86\\64Picking.exe");
shortcut("Exe_65PostProcess",			"Samples\\Bin\\x86\\65PostProcess.exe");
shortcut("Exe_66IngameGUI",				"Samples\\Bin\\x86\\66IngameGUI.exe");
shortcut("Exe_67Game2D",				"Samples\\Bin\\x86\\67Game2D.exe");
shortcut("Exe_70SPARK",					"Samples\\Bin\\x86\\70SPARK.exe");
shortcut("Exe_71libRocket",				"Samples\\Bin\\x86\\71libRocket.exe");
// Tools
shortcut("Exe_PLViewer",				"Tools\\x86\\PLViewer.exe");


// Scene text shortcuts
text_shortcut("Scene_AnchorCamera",					"Samples\\Bin\\Data\\Scenes\\AnchorCamera.scene");
text_shortcut("Scene_AutomaticMeshes",				"Samples\\Bin\\Data\\Scenes\\AutomaticMeshes.scene");
text_shortcut("Scene_AutomaticTextures",			"Samples\\Bin\\Data\\Scenes\\AutomaticTextures.scene");
text_shortcut("Scene_CameraMoveController",			"Samples\\Bin\\Data\\Scenes\\CameraMoveController.scene");
text_shortcut("Scene_MoveController",				"Samples\\Bin\\Data\\Scenes\\MoveController.scene");
text_shortcut("Scene_CharacterAnimation",			"Samples\\Bin\\Data\\Scenes\\CharacterAnimation.scene");
text_shortcut("Scene_MultiCameras",					"Samples\\Bin\\Data\\Scenes\\MultiCameras.scene");
text_shortcut("Scene_NormalMapping",				"Samples\\Bin\\Data\\Scenes\\NormalMapping.scene");
text_shortcut("Scene_OrbiterCamera",				"Samples\\Bin\\Data\\Scenes\\OrbiterCamera.scene");
text_shortcut("Scene_PhysicsCharacterController",	"Samples\\Bin\\Data\\Scenes\\PhysicsCharacterController.scene");
text_shortcut("Scene_PhysicsFirstPersonController",	"Samples\\Bin\\Data\\Scenes\\PhysicsFirstPersonController.scene");
text_shortcut("Scene_PhysicsJoints",				"Samples\\Bin\\Data\\Scenes\\PhysicsJoints.scene");
text_shortcut("Scene_PhysicsManiacMarble",			"Samples\\Bin\\Data\\Scenes\\PhysicsManiacMarble.scene");
text_shortcut("Scene_PhysicsMouseInteraction",		"Samples\\Bin\\Data\\Scenes\\PhysicsMouseInteraction.scene");
text_shortcut("Scene_PLParticleGroups",				"Samples\\Bin\\Data\\Scenes\\PLParticleGroups.scene");
text_shortcut("Scene_Mipmaps",						"Samples\\Bin\\Data\\Scenes\\Mipmaps.scene");
text_shortcut("Scene_ReflectionCubeMap",			"Samples\\Bin\\Data\\Scenes\\ReflectionCubeMap.scene");
text_shortcut("Scene_SceneNodeModifiers",			"Samples\\Bin\\Data\\Scenes\\SceneNodeModifiers.scene");
text_shortcut("Scene_Scripts",						"Samples\\Bin\\Data\\Scenes\\Scripts.scene");


// Script text shortcuts
text_shortcut("Script_Lua_45ScriptApplication", "Samples\\Bin\\Data\\Scripts\\45ScriptApplication.lua");


// Scene exe shortcuts
scene_exe_shortcut("SceneExe_AnchorCamera",					"Samples\\Bin\\Data\\Scenes\\AnchorCamera.scene");
scene_exe_shortcut("SceneExe_AutomaticMeshes",				"Samples\\Bin\\Data\\Scenes\\AutomaticMeshes.scene");
scene_exe_shortcut("SceneExe_AutomaticTextures",			"Samples\\Bin\\Data\\Scenes\\AutomaticTextures.scene");
scene_exe_shortcut("SceneExe_CameraMoveController",			"Samples\\Bin\\Data\\Scenes\\CameraMoveController.scene");
scene_exe_shortcut("SceneExe_CharacterAnimation",			"Samples\\Bin\\Data\\Scenes\\CharacterAnimation.scene");
scene_exe_shortcut("SceneExe_MoveController",				"Samples\\Bin\\Data\\Scenes\\MoveController.scene");
scene_exe_shortcut("SceneExe_MultiCameras",					"Samples\\Bin\\Data\\Scenes\\MultiCameras.scene");
scene_exe_shortcut("SceneExe_NormalMapping",				"Samples\\Bin\\Data\\Scenes\\NormalMapping.scene");
scene_exe_shortcut("SceneExe_OrbiterCamera",				"Samples\\Bin\\Data\\Scenes\\OrbiterCamera.scene");
scene_exe_shortcut("SceneExe_PhysicsCharacterController",	"Samples\\Bin\\Data\\Scenes\\PhysicsCharacterController.scene");
scene_exe_shortcut("SceneExe_PhysicsFirstPersonController",	"Samples\\Bin\\Data\\Scenes\\PhysicsFirstPersonController.scene");
scene_exe_shortcut("SceneExe_PhysicsJoints",				"Samples\\Bin\\Data\\Scenes\\PhysicsJoints.scene");
scene_exe_shortcut("SceneExe_PhysicsManiacMarble",			"Samples\\Bin\\Data\\Scenes\\PhysicsManiacMarble.scene");
scene_exe_shortcut("SceneExe_PhysicsMouseInteraction",		"Samples\\Bin\\Data\\Scenes\\PhysicsMouseInteraction.scene");
scene_exe_shortcut("SceneExe_PLParticleGroups",				"Samples\\Bin\\Data\\Scenes\\PLParticleGroups.scene");
scene_exe_shortcut("SceneExe_Mipmaps",						"Samples\\Bin\\Data\\Scenes\\Mipmaps.scene");
scene_exe_shortcut("SceneExe_ReflectionCubeMap",			"Samples\\Bin\\Data\\Scenes\\ReflectionCubeMap.scene");
scene_exe_shortcut("SceneExe_SceneNodeModifiers",			"Samples\\Bin\\Data\\Scenes\\SceneNodeModifiers.scene");
scene_exe_shortcut("SceneExe_Scripts",						"Samples\\Bin\\Data\\Scenes\\Scripts.scene");
