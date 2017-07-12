//-----------------------------------------------------------------------------
//  File:       publishBtnMqtt.cs
//  
//	Author:		Edward U. Benitez Rendon
//  
//  Date:		11.07.2017
//  
//	Desc:		The file contains functions to send the BGR color that the user
//              select from sliders by mqtt protocol to slave device that set
//              the color in a RGB led. The trigger to publish the mqtt payload 
//              is the OnButtonPressed event of a virtual button.
//-----------------------------------------------------------------------------
using UnityEngine.UI;
using UnityEngine;
// MQTT libraries
using uPLibrary.Networking.M2Mqtt;
using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt.Messages;
using Vuforia;

public class publishBtnMqtt : MonoBehaviour, IVirtualButtonEventHandler {

    private MqttClient4Unity client;
    // Data to connect with the mqtt broker
    public string brokerHostname = "255.255.255.255";
    public int brokerPort = 1883;
    public string usrNm = null;
    public string psw = null;
    public string topic = "zone/sensor";

    public string virtualBtn = "setBtn";
    public string sldrBlueColor = "sliderBlue";
    public string sldrGreenColor = "sliderGreen";
    public string sldrRedColor = "sliderRed";
    public string pnlColor = "panelColor";
    private uint payload = 0;

    // Virtual Btn
    private GameObject vBtnObject;
    // Sliders 
    private GameObject sldrBlue;
    private GameObject sldrGreen;
    private GameObject sldrRed;
    private GameObject showColor;
    // Variables to store the current value from sliders
    private byte blueColor = 0;
    private byte greenColor = 0;
    private byte redColor = 0;

    // Use this for initialization
    void Start ()
    {
        // Link sliders
        sldrBlue = GameObject.Find(sldrBlueColor);
        sldrGreen = GameObject.Find(sldrGreenColor);
        sldrRed = GameObject.Find(sldrRedColor);
        showColor = GameObject.Find(pnlColor);
        // Set the Virtual Btn 
        vBtnObject = GameObject.Find(virtualBtn);
        vBtnObject.GetComponent<VirtualButtonBehaviour>().RegisterEventHandler(this);
        // Connect to the MQTT broker
        if (brokerHostname != null && usrNm != null && psw != null)
        {
            Connect();
        }
    }

    /// <summary>
    /// Update current values from sliders in a variable. The variable
    /// stores the BGR color.
    /// </summary>
    public void setColor()
    {
        // Get the current value from sliders when the IDE detect the OnValueChanged event from sliders
        blueColor = (byte)sldrBlue.GetComponent<Slider>().value;
        greenColor = (byte)sldrGreen.GetComponent<Slider>().value;
        redColor = (byte)sldrRed.GetComponent<Slider>().value;
        Color setColor = new Color32(redColor, greenColor, blueColor, 255);
        UnityEngine.UI.Image showImg = showColor.GetComponent<UnityEngine.UI.Image>();
        showImg.color = setColor;
        // Create a number to store the specified color by the user
        payload = (uint)blueColor << 16;
        payload += ((uint)greenColor << 8);
        payload += redColor;
    }

    /// <summary>
    /// Conect mqtt client to the broker
    /// </summary>
    public void Connect()
    {
        client = new MqttClient4Unity(brokerHostname, brokerPort, false, null);
        string clientId = Guid.NewGuid().ToString();
        client.Connect(clientId, usrNm, psw);
    }

    /// <summary>
    /// Publish payload to the broker
    /// </summary>
    /// <param name="_topic">The topic to send</param>
    /// <param name="msg">Payload</param>
    public void Publish(string _topic, string msg)
    {
        client.Publish(_topic, Encoding.UTF8.GetBytes(msg), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
    }

    /// <summary>
    /// When the virtual button is pressed the function sends the BGR color to the slave device by
    /// mqtt protocol. The color is in hex format.
    /// </summary>
    /// <param name="vb"></param>
    public void OnButtonPressed(VirtualButtonAbstractBehaviour vb)
    {
        string _msg = payload.ToString("x");
        client.Publish(topic, System.Text.Encoding.ASCII.GetBytes(_msg));
        Debug.Log(_msg);
        // throw new NotImplementedException();
    }

    public void OnButtonReleased(VirtualButtonAbstractBehaviour vb)
    {
        //throw new NotImplementedException();
    }
}
