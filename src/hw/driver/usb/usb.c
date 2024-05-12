#include "usb.h"

#include "tusb.h"
#include "usb_desc.h"


static void usbInitPhy(void);




bool usbInit(void)
{
  usbInitPhy();

  tusb_init();

  return true;
}

void usbDeInit(void)
{
  __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
  __HAL_RCC_USBPHYC_CLK_DISABLE();

  /* USB_OTG_HS interrupt Deinit */
  HAL_NVIC_DisableIRQ(OTG_HS_IRQn);  
}

void usbUpdate(void)
{
  tud_task();
}

void tud_mount_cb(void)
{
  logPrintf("tud_mount_cb()\n");
}

void tud_umount_cb(void)
{
  logPrintf("tud_umount_cb()\n");
}

void usbInitPhy(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};


  __HAL_RCC_SYSCFG_CLK_ENABLE();

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USBPHY;
  PeriphClkInit.UsbPhyClockSelection = RCC_USBPHYCLKSOURCE_HSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set the OTG PHY reference clock selection
   */
  HAL_SYSCFG_SetOTGPHYReferenceClockSelection(SYSCFG_OTG_HS_PHY_CLK_SELECT_1);

  /* USB_OTG_HS clock enable */
  __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
  __HAL_RCC_USBPHYC_CLK_ENABLE();

  /* Enable VDDUSB */
  if (__HAL_RCC_PWR_IS_CLK_DISABLED())
  {
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddUSB();

    /*configure VOSR register of USB*/
    HAL_PWREx_EnableUSBHSTranceiverSupply();
    __HAL_RCC_PWR_CLK_DISABLE();
  }
  else
  {
    HAL_PWREx_EnableVddUSB();

    /*configure VOSR register of USB*/
    HAL_PWREx_EnableUSBHSTranceiverSupply();
  }

  /*Configuring the SYSCFG registers OTG_HS PHY*/
  /*OTG_HS PHY enable*/
  HAL_SYSCFG_EnableOTGPHY(SYSCFG_OTG_HS_PHY_ENABLE);

  // Disable VBUS sense (B device)
  USB_OTG_HS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

  // B-peripheral session valid override enable
  USB_OTG_HS->GCCFG |= USB_OTG_GCCFG_VBVALEXTOEN;
  USB_OTG_HS->GCCFG |= USB_OTG_GCCFG_VBVALOVAL;
}

void OTG_HS_IRQHandler(void)
{
  tud_int_handler(0);
}